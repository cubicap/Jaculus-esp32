#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class InputBuilder {
    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Input>(thisVal);
        builder.text(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Input>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue type(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Input>(thisVal);
        builder.type(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Input>(thisVal);
        builder.disabled(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Input>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Input, builder::Input, Input>;

        if(strcmp(name, "text") == 0) return text;
        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "type") == 0) return type;
        if(strcmp(name, "disabled") == 0) return disabled;

        if(strcmp(name, "onChanged") == 0) return &builderCallbackImpl<builder::Input, Input, &builder::Input::onChanged>;

        return nullptr;
    }
};

};
