#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class OrientationWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue yaw(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        return jac::Value::from(ctx_, widget.yaw()).loot().second;
    }

    static JSValue pitch(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        return jac::Value::from(ctx_, widget.pitch()).loot().second;
    }

    static JSValue roll(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        return jac::Value::from(ctx_, widget.roll()).loot().second;
    }

    static JSValue joystickX(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        return jac::Value::from(ctx_, widget.joystickX()).loot().second;
    }

    static JSValue joystickY(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Orientation>(thisVal);
        return jac::Value::from(ctx_, widget.joystickY()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "color") == 0) {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(strcmp(name, "yaw") == 0) {
            *getter = yaw;
            return;
        }
        if(strcmp(name, "pitch") == 0) {
            *getter = pitch;
            return;
        }
        if(strcmp(name, "roll") == 0) {
            *getter = roll;
            return;
        }
        if(strcmp(name, "joystickX") == 0) {
            *getter = joystickX;
            return;
        }
        if(strcmp(name, "joystickY") == 0) {
            *getter = joystickY;
            return;
        }
    }
};

};
