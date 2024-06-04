#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class TextWidget {
    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setText(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setFontSize(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setBackground(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setBackground(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue background(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.background()).loot().second;
    }

    static JSValue setAlign(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setAlign(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue align(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.align()).loot().second;
    }

    static JSValue setValign(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setValign(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue valign(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.valign()).loot().second;
    }

    static JSValue setPrefix(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setPrefix(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue prefix(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.prefix()).loot().second;
    }

    static JSValue setSuffix(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setSuffix(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue suffix(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.suffix()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setText", jac::Value(ctx, JS_NewCFunction(ctx, setText, "setText", 0)));
        proto.set("text", jac::Value(ctx, JS_NewCFunction(ctx, text, "text", 0)));
        proto.set("setFontSize", jac::Value(ctx, JS_NewCFunction(ctx, setFontSize, "setFontSize", 0)));
        proto.set("fontSize", jac::Value(ctx, JS_NewCFunction(ctx, fontSize, "fontSize", 0)));
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("setBackground", jac::Value(ctx, JS_NewCFunction(ctx, setBackground, "setBackground", 0)));
        proto.set("background", jac::Value(ctx, JS_NewCFunction(ctx, background, "background", 0)));
        proto.set("setAlign", jac::Value(ctx, JS_NewCFunction(ctx, setAlign, "setAlign", 0)));
        proto.set("align", jac::Value(ctx, JS_NewCFunction(ctx, align, "align", 0)));
        proto.set("setValign", jac::Value(ctx, JS_NewCFunction(ctx, setValign, "setValign", 0)));
        proto.set("valign", jac::Value(ctx, JS_NewCFunction(ctx, valign, "valign", 0)));
        proto.set("setPrefix", jac::Value(ctx, JS_NewCFunction(ctx, setPrefix, "setPrefix", 0)));
        proto.set("prefix", jac::Value(ctx, JS_NewCFunction(ctx, prefix, "prefix", 0)));
        proto.set("setSuffix", jac::Value(ctx, JS_NewCFunction(ctx, setSuffix, "setSuffix", 0)));
        proto.set("suffix", jac::Value(ctx, JS_NewCFunction(ctx, suffix, "suffix", 0)));
        return proto;
    }
};

};
