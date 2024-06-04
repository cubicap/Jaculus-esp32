#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class SelectWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setBackground(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setBackground(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue background(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.background()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setDisabled(val.to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

    static JSValue setOptions(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setOptions(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue options(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.options()).loot().second;
    }

    static JSValue setSelectedIndex(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setSelectedIndex(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue selectedIndex(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.selectedIndex()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("setBackground", jac::Value(ctx, JS_NewCFunction(ctx, setBackground, "setBackground", 0)));
        proto.set("background", jac::Value(ctx, JS_NewCFunction(ctx, background, "background", 0)));
        proto.set("setDisabled", jac::Value(ctx, JS_NewCFunction(ctx, setDisabled, "setDisabled", 0)));
        proto.set("disabled", jac::Value(ctx, JS_NewCFunction(ctx, disabled, "disabled", 0)));
        proto.set("setOptions", jac::Value(ctx, JS_NewCFunction(ctx, setOptions, "setOptions", 0)));
        proto.set("options", jac::Value(ctx, JS_NewCFunction(ctx, options, "options", 0)));
        proto.set("setSelectedIndex", jac::Value(ctx, JS_NewCFunction(ctx, setSelectedIndex, "setSelectedIndex", 0)));
        proto.set("selectedIndex", jac::Value(ctx, JS_NewCFunction(ctx, selectedIndex, "selectedIndex", 0)));
        return proto;
    }
};

};
