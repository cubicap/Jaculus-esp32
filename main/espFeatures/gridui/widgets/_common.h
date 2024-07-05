#pragma once

#include <jac/machine/values.h>
#include <map>
#include <functional>
#include <atomic>

namespace gridui_jac {

enum class WidgetTypeId : uint16_t {
    Base,

    Arm,
    Bar,
    Button,
    Camera,
    Checkbox,
    Circle,
    Input,
    Joystick,
    Led,
    Orientation,
    Select,
    Slider,
    SpinEdit,
    Switcher,
    Text,
};

class GridUiContext {
    struct __attribute__ ((packed)) HandlerWidgetItem {
        JSValue object;
        uint16_t uuid;
    };
    std::vector<HandlerWidgetItem> _handlerWidgets;

    std::function<void(std::function<void()>)> _scheduleEvent;

    static JSClassID _builderClassId;
    static JSClassID _widgetClassId;

    static JSClassExoticMethods builderExotic;
    static int builderGetOwnProperty(JSContext *ctx, JSPropertyDescriptor *desc, JSValueConst obj, JSAtom prop);
    static JSClassExoticMethods widgetExotic;
    static int widgetGetOwnProperty(JSContext *ctx, JSPropertyDescriptor *desc, JSValueConst obj, JSAtom prop);

    GridUiContext() {}

public:
    static GridUiContext& get() {
        static GridUiContext instance;
        return instance;
    }

    void setScheduleEvent(std::function<void(std::function<void()>)> schedule) {
        _scheduleEvent = schedule;
    }

    void scheduleEvent(std::function<void()> event) {
        if(_scheduleEvent) {
            _scheduleEvent(event);
        }
    }

    void createClasses(jac::ContextRef context);
    static JSClassID builderClassId() { return _builderClassId; }
    static JSClassID widgetClassId() { return _widgetClassId; }

    void clear(jac::ContextRef ctx) {
        clearProtos();

        for(auto& handle : _handlerWidgets) {
            JS_FreeValue(ctx.get(), handle.object);
        }
        _handlerWidgets.clear();
        _handlerWidgets.shrink_to_fit();
    }

    void clearProtos() {
        _handlerWidgets.shrink_to_fit();
    }

    void addHandlerWidget(uint16_t uuid, jac::Object obj) {
        for(const auto& handle : _handlerWidgets) {
            if(handle.uuid == uuid) {
                return;
            }
        }
        _handlerWidgets.push_back({
            .object = obj.loot().second,
            .uuid = uuid,
        });
    }

    jac::Value getHandlerWidget(jac::ContextRef ctx, uint16_t uuid) {
        for(const auto& handle : _handlerWidgets) {
            if(handle.uuid == uuid) {
                return jac::Object(ctx, JS_DupValue(ctx.get(), handle.object));
            }
        }
        return jac::Value::undefined(ctx);
    }
};

template<int size>
inline bool literalEqual(const char *c_str, int c_str_size, const char (&literal)[size]) {
    return c_str_size == size-1 && memcmp(c_str, literal, size-1) == 0;
}

class AtomString {
    JSContext *_ctx;
    const char *_value;
    int _valueLen;

    AtomString(AtomString&) = delete;
public:
    AtomString(JSContext *ctx, JSAtom atom) : _ctx(ctx), _value(JS_AtomToCString(ctx, atom)) {
        _valueLen = strlen(_value);
    }

    ~AtomString() {
        JS_FreeCString(_ctx, _value);
    }

    const char *c_str() const { return _value; }

    template<int size>
    inline bool operator==(const char (&literal)[size]) const {
        return literalEqual<size>(_value, _valueLen, literal);
    }
};

template<typename BuilderT>
inline BuilderT& builderOpaque(JSValueConst thisVal) {
    return *reinterpret_cast<BuilderT*>(JS_GetOpaque(thisVal, GridUiContext::builderClassId()));
}


template<typename WidgetT>
inline WidgetT& widgetOpaque(JSValueConst thisVal) {
    return *reinterpret_cast<WidgetT*>(JS_GetOpaque(thisVal, GridUiContext::widgetClassId()));
}


using qjsGetter = JSValue(*)(JSContext* ctx_, JSValueConst thisVal);
using qjsSetter = JSValue(*)(JSContext* ctx_, JSValueConst thisVal, JSValueConst val);

template<typename BuilderT, typename WidgetT, auto setter>
JSValue builderCallbackImpl(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
    auto *builder = reinterpret_cast<BuilderT*>(JS_GetOpaque(thisVal, GridUiContext::builderClassId()));
    auto callback = jac::Function(ctx_, JS_DupValue(ctx_, argv[0]));

    (builder->*setter)([ctx_, callback](WidgetT& widget) {
        const auto uuid = widget.uuid();
        GridUiContext::get().scheduleEvent([ctx_, callback, uuid]() mutable {
            auto obj = GridUiContext::get().getHandlerWidget(ctx_, uuid);
            callback.call<void>(obj);
        });
    });
    return JS_DupValue(ctx_, thisVal);
}

template<typename BuilderT>
JSValue builderCss(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
    auto& builder = *reinterpret_cast<BuilderT*>(JS_GetOpaque(thisVal, GridUiContext::builderClassId()));
    builder.css(jac::ValueWeak(ctx_, argv[0]).to<std::string>(), jac::ValueWeak(ctx_, argv[1]).to<std::string>());
    return JS_DupValue(ctx_, thisVal);
}


template<WidgetTypeId typeId, typename BuilderT, typename WidgetT>
JSValue builderFinish(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
    auto& builder = *reinterpret_cast<BuilderT*>(JS_GetOpaque(thisVal, GridUiContext::builderClassId()));

    auto widget = new WidgetT(std::move(builder.finish()));

    auto proto = jac::Value(ctx_, JS_GetClassProto(ctx_, GridUiContext::widgetClassId()));

    JSValue obj = JS_NewObjectProtoClass(ctx_, proto.getVal(), GridUiContext::widgetClassId());

    JS_SetOpaque(obj, widget);
    JS_DefinePropertyValueUint32(ctx_, obj, 0, JS_NewUint32(ctx_, static_cast<uint32_t>(typeId)), JS_PROP_NO_EXOTIC);

    if(widget->hasRegisteredCallbacks()) {
        GridUiContext::get().addHandlerWidget(widget->uuid(), jac::Object(ctx_, JS_DupValue(ctx_, obj)));
    }

    return obj;
}

};
