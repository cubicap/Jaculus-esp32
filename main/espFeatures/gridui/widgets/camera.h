#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class CameraWidget {
    static JSValue setRotation(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        widget.setRotation(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue rotation(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.rotation()).loot().second;
    }

    static JSValue setClip(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        widget.setClip(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue clip(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.clip()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetProperty(ctx, proto, "rotation", "setRotation", rotation, setRotation);
        defineWidgetProperty(ctx, proto, "clip", "setClip", clip, setClip);
        return proto;
    }
};

};
