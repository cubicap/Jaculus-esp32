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
    static void getProperty(const AtomString& name, qjsGetter* getter, qjsSetter *setter) {
        if(name == "color") {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(name == "yaw") {
            *getter = yaw;
            return;
        }
        if(name == "pitch") {
            *getter = pitch;
            return;
        }
        if(name == "roll") {
            *getter = roll;
            return;
        }
        if(name == "joystickX") {
            *getter = joystickX;
            return;
        }
        if(name == "joystickY") {
            *getter = joystickY;
            return;
        }
    }
};

};
