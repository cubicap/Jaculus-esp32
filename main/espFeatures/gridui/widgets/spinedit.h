#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class SpinEditWidget {
    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setValue(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        widget.setValue(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue value(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        return jac::Value::from(ctx_, widget.value()).loot().second;
    }

    static JSValue setStep(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        widget.setStep(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue step(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        return jac::Value::from(ctx_, widget.step()).loot().second;
    }

    static JSValue setPrecision(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        widget.setPrecision(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue precision(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::SpinEdit>(thisVal);
        return jac::Value::from(ctx_, widget.precision()).loot().second;
    }

public:
    static void getProperty(const AtomString& name, qjsGetter* getter, qjsSetter *setter) {
        if(name == "fontSize") {
            *getter = fontSize;
            *setter = setFontSize;
            return;
        }
        if(name == "color") {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(name == "value") {
            *getter = value;
            *setter = setValue;
            return;
        }
        if(name == "step") {
            *getter = step;
            *setter = setStep;
            return;
        }
        if(name == "precision") {
            *getter = precision;
            *setter = setPrecision;
            return;
        }
    }
};

};
