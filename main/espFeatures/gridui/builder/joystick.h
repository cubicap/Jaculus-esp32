#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class JoystickBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Joystick>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue keys(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Joystick>(thisVal);
        builder.keys(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Joystick>(thisVal);
        builder.text(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Joystick>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Joystick, builder::Joystick, Joystick>;

        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "keys") == 0) return keys;
        if(strcmp(name, "text") == 0) return text;

        if(strcmp(name, "onClick") == 0) return &builderCallbackImpl<builder::Joystick, Joystick, &builder::Joystick::onClick>;
        if(strcmp(name, "onPositionChanged") == 0) return &builderCallbackImpl<builder::Joystick, Joystick, &builder::Joystick::onPositionChanged>;

        return nullptr;
    }
};

};
