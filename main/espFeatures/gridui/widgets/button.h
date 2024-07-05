#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class ButtonWidget {
    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setText(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setBackground(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setBackground(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue background(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.background()).loot().second;
    }

    static JSValue setAlign(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setAlign(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue align(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.align()).loot().second;
    }

    static JSValue setValign(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setValign(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue valign(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.valign()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        widget.setDisabled(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

    static JSValue pressed(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Button>(thisVal);
        return jac::Value::from(ctx_, widget.pressed()).loot().second;
    }

public:
    static void getProperty(const AtomString& name, qjsGetter* getter, qjsSetter *setter) {
        if(name == "text") {
            *getter = text;
            *setter = setText;
            return;
        }
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
        if(name == "background") {
            *getter = background;
            *setter = setBackground;
            return;
        }
        if(name == "align") {
            *getter = align;
            *setter = setAlign;
            return;
        }
        if(name == "valign") {
            *getter = valign;
            *setter = setValign;
            return;
        }
        if(name == "disabled") {
            *getter = disabled;
            *setter = setDisabled;
            return;
        }
        if(name == "pressed") {
            *getter = pressed;
            return;
        }
    }
};

};
