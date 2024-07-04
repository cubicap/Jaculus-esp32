#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class JoystickWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setKeys(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        widget.setKeys(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue keys(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        return jac::Value::from(ctx_, widget.keys()).loot().second;
    }

    static JSValue setText(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        widget.setText(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue text(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        return jac::Value::from(ctx_, widget.text()).loot().second;
    }

    static JSValue x(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        return jac::Value::from(ctx_, widget.x()).loot().second;
    }

    static JSValue y(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Joystick>(thisVal);
        return jac::Value::from(ctx_, widget.y()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "color") == 0) {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(strcmp(name, "keys") == 0) {
            *getter = keys;
            *setter = setKeys;
            return;
        }
        if(strcmp(name, "text") == 0) {
            *getter = text;
            *setter = setText;
            return;
        }
        if(strcmp(name, "x") == 0) {
            *getter = x;
            return;
        }
        if(strcmp(name, "y") == 0) {
            *getter = y;
            return;
        }
    }
};

};
