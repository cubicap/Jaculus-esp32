#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class ButtonWidget {
    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setText(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setBackground(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setBackground(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue background(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.background()).loot().second;
    }

    static JSValue setAlign(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setAlign(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue align(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.align()).loot().second;
    }

    static JSValue setValign(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setValign(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue valign(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.valign()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        widget.setDisabled(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Button*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "text", "setText", text, setText);
        defineWidgetProperty(ctx, proto, "fontSize", "setFontSize", fontSize, setFontSize);
        defineWidgetProperty(ctx, proto, "color", "setColor", color, setColor);
        defineWidgetProperty(ctx, proto, "background", "setBackground", background, setBackground);
        defineWidgetProperty(ctx, proto, "align", "setAlign", align, setAlign);
        defineWidgetProperty(ctx, proto, "valign", "setValign", valign, setValign);
        defineWidgetProperty(ctx, proto, "disabled", "setDisabled", disabled, setDisabled);
        return proto;
    }
};

};
