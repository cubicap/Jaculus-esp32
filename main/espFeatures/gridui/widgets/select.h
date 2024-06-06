#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class SelectWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setBackground(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        widget.setBackground(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue background(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.background()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        widget.setDisabled(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

    static JSValue setOptions(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        widget.setOptions(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue options(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.options()).loot().second;
    }

    static JSValue setSelectedIndex(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        widget.setSelectedIndex(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue selectedIndex(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.selectedIndex()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "color", "setColor", color, setColor);
        defineWidgetProperty(ctx, proto, "background", "setBackground", background, setBackground);
        defineWidgetProperty(ctx, proto, "disabled", "setDisabled", disabled, setDisabled);
        defineWidgetProperty(ctx, proto, "options", "setOptions", options, setOptions);
        defineWidgetProperty(ctx, proto, "selectedIndex", "setSelectedIndex", selectedIndex, setSelectedIndex);
        return proto;
    }
};

};
