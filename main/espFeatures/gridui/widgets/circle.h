#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class CircleWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setMin(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setMin(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue min(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.min()).loot().second;
    }

    static JSValue setMax(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setMax(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue max(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.max()).loot().second;
    }

    static JSValue setLineWidth(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setLineWidth(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue lineWidth(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.lineWidth()).loot().second;
    }

    static JSValue setValueStart(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setValueStart(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue valueStart(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.valueStart()).loot().second;
    }

    static JSValue setValue(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setValue(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue value(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.value()).loot().second;
    }

    static JSValue setShowValue(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        widget.setShowValue(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue showValue(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Circle>(thisVal);
        return jac::Value::from(ctx_, widget.showValue()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "color") == 0) {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(strcmp(name, "fontSize") == 0) {
            *getter = fontSize;
            *setter = setFontSize;
            return;
        }
        if(strcmp(name, "min") == 0) {
            *getter = min;
            *setter = setMin;
            return;
        }
        if(strcmp(name, "max") == 0) {
            *getter = max;
            *setter = setMax;
            return;
        }
        if(strcmp(name, "lineWidth") == 0) {
            *getter = lineWidth;
            *setter = setLineWidth;
            return;
        }
        if(strcmp(name, "valueStart") == 0) {
            *getter = valueStart;
            *setter = setValueStart;
            return;
        }
        if(strcmp(name, "value") == 0) {
            *getter = value;
            *setter = setValue;
            return;
        }
        if(strcmp(name, "showValue") == 0) {
            *getter = showValue;
            *setter = setShowValue;
            return;
        }
    }
};

};
