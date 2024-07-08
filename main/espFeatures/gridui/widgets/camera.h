#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class CameraWidget {
    static JSValue setRotation(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Camera>(thisVal);
        widget.setRotation(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue rotation(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Camera>(thisVal);
        return jac::Value::from(ctx_, widget.rotation()).loot().second;
    }

    static JSValue setClip(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Camera>(thisVal);
        widget.setClip(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue clip(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Camera>(thisVal);
        return jac::Value::from(ctx_, widget.clip()).loot().second;
    }

public:
    static void getProperty(const AtomString& name, qjsGetter* getter, qjsSetter *setter) {
        if(name == "rotation") {
            *getter = rotation;
            *setter = setRotation;
            return;
        }
        if(name == "clip") {
            *getter = clip;
            *setter = setClip;
            return;
        }
    }
};

};
