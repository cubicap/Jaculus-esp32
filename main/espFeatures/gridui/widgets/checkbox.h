#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class CheckboxWidget {
    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        widget.setFontSize(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setChecked(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        widget.setChecked(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue checked(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        return jac::Value::from(ctx_, widget.checked()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        widget.setText(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Checkbox>(thisVal);
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "fontSize") == 0) {
            *getter = fontSize;
            *setter = setFontSize;
            return;
        }
        if(strcmp(name, "checked") == 0) {
            *getter = checked;
            *setter = setChecked;
            return;
        }
        if(strcmp(name, "color") == 0) {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(strcmp(name, "text") == 0) {
            *getter = text;
            *setter = setText;
            return;
        }
    }
};

};
