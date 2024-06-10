#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class JoystickBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal, 1));
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue keys(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal, 1));
        builder.keys(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal, 1));
        builder.text(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 1)));
        proto.set("keys", jac::Value(ctx, JS_NewCFunction(ctx, keys, "keys", 1)));
        proto.set("text", jac::Value(ctx, JS_NewCFunction(ctx, text, "text", 1)));

        using namespace gridui;
        defineBuilderCallback<builder::Joystick, Joystick, &builder::Joystick::onClick>(ctx, proto, "onClick");
        defineBuilderCallback<builder::Joystick, Joystick, &builder::Joystick::onPositionChanged>(ctx, proto, "onPositionChanged");

        return proto;
    }
};

};
