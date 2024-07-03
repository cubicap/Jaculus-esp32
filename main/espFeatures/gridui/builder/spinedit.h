#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class SpinEditBuilder {
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal, 1));
        builder.fontSize(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal, 1));
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal, 1));
        builder.value(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue step(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal, 1));
        builder.step(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue precision(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal, 1));
        builder.precision(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        using namespace gridui;

        auto proto = jac::Object::create(ctx);

        proto.set("fontSize", jac::Value(ctx, JS_NewCFunction(ctx, fontSize, "fontSize", 1)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 1)));
        proto.set("value", jac::Value(ctx, JS_NewCFunction(ctx, value, "value", 1)));
        proto.set("step", jac::Value(ctx, JS_NewCFunction(ctx, step, "step", 1)));
        proto.set("precision", jac::Value(ctx, JS_NewCFunction(ctx, precision, "precision", 1)));

        defineBuilderCallback<builder::SpinEdit, SpinEdit, &builder::SpinEdit::onChanged>(ctx, proto, "onChanged");

        return proto;
    }
};

};
