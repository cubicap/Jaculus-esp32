#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class SpinEditBuilder {
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::SpinEdit>(thisVal);
        builder.fontSize(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::SpinEdit>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::SpinEdit>(thisVal);
        builder.value(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue step(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::SpinEdit>(thisVal);
        builder.step(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue precision(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::SpinEdit>(thisVal);
        builder.precision(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::SpinEdit>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::SpinEdit, builder::SpinEdit, SpinEdit>;

        if(strcmp(name, "fontSize") == 0) return fontSize;
        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "value") == 0) return value;
        if(strcmp(name, "step") == 0) return step;
        if(strcmp(name, "precision") == 0) return precision;

        if(strcmp(name, "onChanged") == 0) return &builderCallbackImpl<builder::SpinEdit, SpinEdit, &builder::SpinEdit::onChanged>;

        return nullptr;
    }
};

};
