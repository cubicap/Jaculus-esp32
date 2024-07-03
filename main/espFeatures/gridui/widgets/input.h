#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class InputWidget {
    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        widget.setText(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setType(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        widget.setType(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue type(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.type()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        widget.setDisabled(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "text", "setText", text, setText);
        defineWidgetProperty(ctx, proto, "color", "setColor", color, setColor);
        defineWidgetProperty(ctx, proto, "type", "setType", type, setType);
        defineWidgetProperty(ctx, proto, "disabled", "setDisabled", disabled, setDisabled);
        return proto;
    }
};

};
