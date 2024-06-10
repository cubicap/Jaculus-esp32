#pragma once

#include <jac/machine/values.h>
#include <map>

namespace gridui_jac {

enum class WidgetTypeId : uint16_t {
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
    static constexpr const uint16_t _builderOffset = 1000;

    std::map<uint16_t, jac::Object> _protoCache;
    std::map<uint16_t, jac::Object> _constructedWidgets;

    std::function<void(std::function<void()>)> _scheduleEvent;

public:
    static GridUiContext& get() {
        static GridUiContext instance;
        return instance;
    }

    void setScheduleEvent(std::function<void(std::function<void()>)> schedule) {
        _scheduleEvent = schedule;
    }

    void scheduleEvent(std::function<void()> event) {
        _scheduleEvent(event);
    }

    void clear() {
        _protoCache.clear();
        _constructedWidgets.clear();
    }

    void clearProtos() {
        _protoCache.clear();
    }

    jac::Object buildObj(jac::ContextRef ctx, WidgetTypeId typeId, bool isBuilder, void *opaque, std::function<jac::Object(jac::ContextRef)> createNewProto) {
        auto cacheKey = static_cast<uint16_t>(typeId);
        if(isBuilder) {
            cacheKey += _builderOffset;
        }

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), opaque);

        auto itr = _protoCache.find(cacheKey);
        if(itr == _protoCache.end()) {
            auto proto = createNewProto(ctx);
            _protoCache.emplace(cacheKey, proto);
            obj.setPrototype(proto);
        } else {
            obj.setPrototype(itr->second);
        }

        if(!isBuilder) {
            auto widget = reinterpret_cast<gridui::Widget*>(opaque);
            _constructedWidgets.insert({widget->uuid(), obj});
        }

        return obj;
    }

    jac::Value getConstructedWidget(jac::ContextRef ctx, uint16_t uuid) {
        auto itr = _constructedWidgets.find(uuid);
        if(itr == _constructedWidgets.end()) {
            return jac::Value::undefined(ctx);
        }
        return itr->second;
    }
};

using qjsGetter = JSValue(*)(JSContext* ctx_, JSValueConst thisVal);
using qjsSetter = JSValue(*)(JSContext* ctx_, JSValueConst thisVal, JSValueConst val);

static inline void defineWidgetProperty(jac::ContextRef ctx, jac::Object& target,
    const char *name, const char *setterName,
    qjsGetter getter, qjsSetter setter) {

    auto jsGetter = JS_NewCFunction2(ctx.get(), reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(getter)), name, 0, JS_CFUNC_getter, 0);
    auto jsSetter = JS_NewCFunction2(ctx.get(), reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(setter)), setterName, 1, JS_CFUNC_setter, 0);

    JS_DefinePropertyGetSet(
        ctx.get(), target.getVal(),
        jac::Atom::create(ctx, name).get(), 
        jsGetter, jsSetter, 0
    );
}

static inline void defineWidgetPropertyReadOnly(jac::ContextRef ctx, jac::Object& target, const char *name, qjsGetter getter) {
    auto jsGetter = JS_NewCFunction2(ctx.get(), reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(getter)), name, 0, JS_CFUNC_getter, 0);
    JS_DefineProperty(ctx, target.getVal(), jac::Atom::create(ctx, name).get(), JS_UNDEFINED, jsGetter, JS_UNDEFINED, JS_PROP_HAS_GET | JS_PROP_HAS_ENUMERABLE);
    JS_FreeValue(ctx, jsGetter);
}

template<typename BuilderT, typename WidgetT, auto setter>
static JSValue builderCallbackImpl(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
    auto *builder = reinterpret_cast<BuilderT*>(JS_GetOpaque(thisVal, 1));
    auto callback = jac::Function(ctx_, JS_DupValue(ctx_, argv[0]));
    (builder->*setter)([=](WidgetT& widget) {
        const auto uuid = widget.uuid();
        GridUiContext::get().scheduleEvent([=]() mutable {
            auto obj = GridUiContext::get().getConstructedWidget(ctx_, uuid);
            callback.call<void>(obj);
        });
    });
    return JS_DupValue(ctx_, thisVal);
}

template<typename BuilderT, typename WidgetT, auto setter>
static inline void defineBuilderCallback(jac::ContextRef ctx, jac::Object& target, const char *name) {
    target.set(name, jac::Value(ctx, JS_NewCFunction(ctx, &builderCallbackImpl<BuilderT, WidgetT, setter>, name, 1)));
}

};
