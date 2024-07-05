#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/_common.h"

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
    static JSCFunction *getPropFunc(const AtomString& name) {
        using namespace gridui;

        if(name == "css") return builderCss<builder::SpinEdit>;
        if(name == "finish") return builderFinish<WidgetTypeId::SpinEdit, builder::SpinEdit, SpinEdit>;

        if(name == "fontSize") return fontSize;
        if(name == "color") return color;
        if(name == "value") return value;
        if(name == "step") return step;
        if(name == "precision") return precision;

        if(name == "onChanged") return &builderCallbackImpl<builder::SpinEdit, SpinEdit, &builder::SpinEdit::onChanged>;

        return nullptr;
    }
};

};
