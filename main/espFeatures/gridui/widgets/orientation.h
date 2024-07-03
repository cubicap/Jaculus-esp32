#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class OrientationWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue yaw(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.yaw()).loot().second;
    }

    static JSValue pitch(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.pitch()).loot().second;
    }

    static JSValue roll(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.roll()).loot().second;
    }

    static JSValue joystickX(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.joystickX()).loot().second;
    }

    static JSValue joystickY(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Orientation*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.joystickY()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "color", "setColor", color, setColor);
        defineWidgetPropertyReadOnly(ctx, proto, "yaw", yaw);
        defineWidgetPropertyReadOnly(ctx, proto, "pitch", pitch);
        defineWidgetPropertyReadOnly(ctx, proto, "roll", roll);
        defineWidgetPropertyReadOnly(ctx, proto, "joystickX", joystickX);
        defineWidgetPropertyReadOnly(ctx, proto, "joystickY", joystickY);
        return proto;
    }
};

};
