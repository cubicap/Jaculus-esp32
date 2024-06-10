#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class BarBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal, 1));
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal, 1));
        builder.fontSize(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue min(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal, 1));
        builder.min(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue max(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal, 1));
        builder.max(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal, 1));
        builder.value(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue showValue(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal, 1));
        builder.showValue(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        using namespace gridui;

        auto proto = jac::Object::create(ctx);

        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 1)));
        proto.set("fontSize", jac::Value(ctx, JS_NewCFunction(ctx, fontSize, "fontSize", 1)));
        proto.set("min", jac::Value(ctx, JS_NewCFunction(ctx, min, "min", 1)));
        proto.set("max", jac::Value(ctx, JS_NewCFunction(ctx, max, "max", 1)));
        proto.set("value", jac::Value(ctx, JS_NewCFunction(ctx, value, "value", 1)));
        proto.set("showValue", jac::Value(ctx, JS_NewCFunction(ctx, showValue, "showValue", 1)));

        return proto;
    }
};

};
