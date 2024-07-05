#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/_common.h"

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
    static JSCFunction *getPropFunc(const AtomString& name) {
        using namespace gridui;

        if(name == "css") return builderCss<builder::Joystick>;
        if(name == "finish") return builderFinish<WidgetTypeId::Joystick, builder::Joystick, Joystick>;

        if(name == "color") return color;
        if(name == "keys") return keys;
        if(name == "text") return text;

        if(name == "onClick") return &builderCallbackImpl<builder::Joystick, Joystick, &builder::Joystick::onClick>;
        if(name == "onPositionChanged") return &builderCallbackImpl<builder::Joystick, Joystick, &builder::Joystick::onPositionChanged>;

        return nullptr;
    }
};

};
