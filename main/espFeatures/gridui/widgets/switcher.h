#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class SwitcherWidget {
    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setFontSize(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.value()).loot().second;
    }

    static JSValue setMin(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setMin(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue min(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.min()).loot().second;
    }

    static JSValue setMax(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setMax(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue max(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.max()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setFontSize", jac::Value(ctx, JS_NewCFunction(ctx, setFontSize, "setFontSize", 0)));
        proto.set("fontSize", jac::Value(ctx, JS_NewCFunction(ctx, fontSize, "fontSize", 0)));
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("value", jac::Value(ctx, JS_NewCFunction(ctx, value, "value", 0)));
        proto.set("setMin", jac::Value(ctx, JS_NewCFunction(ctx, setMin, "setMin", 0)));
        proto.set("min", jac::Value(ctx, JS_NewCFunction(ctx, min, "min", 0)));
        proto.set("setMax", jac::Value(ctx, JS_NewCFunction(ctx, setMax, "setMax", 0)));
        proto.set("max", jac::Value(ctx, JS_NewCFunction(ctx, max, "max", 0)));
        return proto;
    }
};

};
