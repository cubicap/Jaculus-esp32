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
    static void getProperty(const AtomString& name, qjsGetter* getter, qjsSetter *setter) {
        if(name == "text") {
            *getter = text;
            *setter = setText;
            return;
        }
        if(name == "color") {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(name == "type") {
            *getter = type;
            *setter = setType;
            return;
        }
        if(name == "disabled") {
            *getter = disabled;
            *setter = setDisabled;
            return;
        }
    }
};

};
