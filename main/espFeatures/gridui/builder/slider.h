#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class SliderBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.fontSize(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue min(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.min(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue max(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.max(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.value(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue precision(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.precision(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue showValue(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Slider>(thisVal);
        builder.showValue(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Slider>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Slider, builder::Slider, Slider>;

        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "fontSize") == 0) return fontSize;
        if(strcmp(name, "min") == 0) return min;
        if(strcmp(name, "max") == 0) return max;
        if(strcmp(name, "value") == 0) return value;
        if(strcmp(name, "precision") == 0) return precision;
        if(strcmp(name, "showValue") == 0) return showValue;

        if(strcmp(name, "onChanged") == 0) return &builderCallbackImpl<builder::Slider, Slider, &builder::Slider::onChanged>;

        return nullptr;
    }
};

};
