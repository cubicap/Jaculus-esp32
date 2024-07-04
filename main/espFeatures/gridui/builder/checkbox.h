#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class CheckboxBuilder {
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Checkbox>(thisVal);
        builder.fontSize(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue checked(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Checkbox>(thisVal);
        builder.checked(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Checkbox>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Checkbox>(thisVal);
        builder.text(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Checkbox>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Checkbox, builder::Checkbox, Checkbox>;

        if(strcmp(name, "fontSize") == 0) return fontSize;
        if(strcmp(name, "checked") == 0) return checked;
        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "text") == 0) return text;

        if(strcmp(name, "onChanged") == 0) return &builderCallbackImpl<builder::Checkbox, Checkbox, &builder::Checkbox::onChanged>;

        return nullptr;
    }
};

};
