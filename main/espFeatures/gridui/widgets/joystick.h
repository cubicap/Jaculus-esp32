#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class JoystickWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setKeys(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setKeys(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue keys(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.keys()).loot().second;
    }

    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setText(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("setKeys", jac::Value(ctx, JS_NewCFunction(ctx, setKeys, "setKeys", 0)));
        proto.set("keys", jac::Value(ctx, JS_NewCFunction(ctx, keys, "keys", 0)));
        proto.set("setText", jac::Value(ctx, JS_NewCFunction(ctx, setText, "setText", 0)));
        proto.set("text", jac::Value(ctx, JS_NewCFunction(ctx, text, "text", 0)));
        return proto;
    }
};

};
