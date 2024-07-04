#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class SelectWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        widget.setColor(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setBackground(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        widget.setBackground(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue background(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        return jac::Value::from(ctx_, widget.background()).loot().second;
    }

    static JSValue setDisabled(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        widget.setDisabled(jac::ValueWeak(ctx_, val).to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        return jac::Value::from(ctx_, widget.disabled()).loot().second;
    }

    static JSValue setOptions(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        widget.setOptions(jac::ValueWeak(ctx_, val).to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue options(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        return jac::Value::from(ctx_, widget.options()).loot().second;
    }

    static JSValue setSelectedIndex(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        widget.setSelectedIndex(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue selectedIndex(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Select>(thisVal);
        return jac::Value::from(ctx_, widget.selectedIndex()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "color") == 0) {
            *getter = color;
            *setter = setColor;
            return;
        }
        if(strcmp(name, "background") == 0) {
            *getter = background;
            *setter = setBackground;
            return;
        }
        if(strcmp(name, "disabled") == 0) {
            *getter = disabled;
            *setter = setDisabled;
            return;
        }
        if(strcmp(name, "options") == 0) {
            *getter = options;
            *setter = setOptions;
            return;
        }
        if(strcmp(name, "selectedIndex") == 0) {
            *getter = selectedIndex;
            *setter = setSelectedIndex;
            return;
        }
    }
};

};
