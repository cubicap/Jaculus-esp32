#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class SwitcherWidget {
    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue value(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.value()).loot().second;
    }

    static JSValue setMin(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        widget.setMin(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue min(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.min()).loot().second;
    }

    static JSValue setMax(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        widget.setMax(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue max(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Switcher*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.max()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "fontSize", "setFontSize", fontSize, setFontSize);
        defineWidgetProperty(ctx, proto, "color", "setColor", color, setColor);
        defineWidgetPropertyReadOnly(ctx, proto, "value", value);
        defineWidgetProperty(ctx, proto, "min", "setMin", min, setMin);
        defineWidgetProperty(ctx, proto, "max", "setMax", max, setMax);
        return proto;
    }
};

};
