#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class InputWidget {
    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        widget.setText(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setType(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        widget.setType(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue type(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        return jac::Value::from(ctx_, widget.type()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        widget.setDisabled(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Input>(thisVal);
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "text") == 0) {
            *getter = text;
            *setter = setText;
            return;
        }
        if(strcmp(name, "color") == 0) {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(strcmp(name, "type") == 0) {
            *getter = type;
            *setter = setType;
            return;
        }
        if(strcmp(name, "disabled") == 0) {
            *getter = disabled;
            *setter = setDisabled;
            return;
        }
    }
};

};
