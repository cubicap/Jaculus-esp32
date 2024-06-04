#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class InputWidget {
    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setText(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setType(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setType(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue type(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.type()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setDisabled(val.to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setText", jac::Value(ctx, JS_NewCFunction(ctx, setText, "setText", 0)));
        proto.set("text", jac::Value(ctx, JS_NewCFunction(ctx, text, "text", 0)));
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("setType", jac::Value(ctx, JS_NewCFunction(ctx, setType, "setType", 0)));
        proto.set("type", jac::Value(ctx, JS_NewCFunction(ctx, type, "type", 0)));
        proto.set("setDisabled", jac::Value(ctx, JS_NewCFunction(ctx, setDisabled, "setDisabled", 0)));
        proto.set("disabled", jac::Value(ctx, JS_NewCFunction(ctx, disabled, "disabled", 0)));
        return proto;
    }
};

};
