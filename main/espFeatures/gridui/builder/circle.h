#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class CircleBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.fontSize(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue min(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.min(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue max(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.max(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue lineWidth(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.lineWidth(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue valueStart(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.valueStart(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.value(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue showValue(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Circle>(thisVal);
        builder.showValue(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Circle>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Circle, builder::Circle, Circle>;

        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "fontSize") == 0) return fontSize;
        if(strcmp(name, "min") == 0) return min;
        if(strcmp(name, "max") == 0) return max;
        if(strcmp(name, "lineWidth") == 0) return lineWidth;
        if(strcmp(name, "valueStart") == 0) return valueStart;
        if(strcmp(name, "value") == 0) return value;
        if(strcmp(name, "showValue") == 0) return showValue;

        return nullptr;
    }
};

};
