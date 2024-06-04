#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class TextBuilder {
    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.text(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.fontSize(val.to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.color(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue background(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.background(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue align(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.align(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue valign(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.valign(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue prefix(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.prefix(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue suffix(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.suffix(val.to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("text", jac::Value(ctx, JS_NewCFunction(ctx, text, "text", 0)));
        proto.set("fontSize", jac::Value(ctx, JS_NewCFunction(ctx, fontSize, "fontSize", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("background", jac::Value(ctx, JS_NewCFunction(ctx, background, "background", 0)));
        proto.set("align", jac::Value(ctx, JS_NewCFunction(ctx, align, "align", 0)));
        proto.set("valign", jac::Value(ctx, JS_NewCFunction(ctx, valign, "valign", 0)));
        proto.set("prefix", jac::Value(ctx, JS_NewCFunction(ctx, prefix, "prefix", 0)));
        proto.set("suffix", jac::Value(ctx, JS_NewCFunction(ctx, suffix, "suffix", 0)));
        return proto;
    }
};

};
