#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class BaseWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setMin(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        widget.setMin(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue min(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.min()).loot().second;
    }

    static JSValue setMax(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        widget.setMax(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue max(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.max()).loot().second;
    }

    static JSValue setValue(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        widget.setValue(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue value(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.value()).loot().second;
    }

    static JSValue setShowValue(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        widget.setShowValue(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue showValue(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.showValue()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "color", "setColor", color, setColor);
        defineWidgetProperty(ctx, proto, "fontSize", "setFontSize", fontSize, setFontSize);
        defineWidgetProperty(ctx, proto, "min", "setMin", min, setMin);
        defineWidgetProperty(ctx, proto, "max", "setMax", max, setMax);
        defineWidgetProperty(ctx, proto, "value", "setValue", value, setValue);
        defineWidgetProperty(ctx, proto, "showValue", "setShowValue", showValue, setShowValue);
        return proto;
    }
};

};
