#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class WidgetWidget {
    static JSValue uuid(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        return jac::Value::from(ctx_, widget.uuid()).loot().second;
    }

    static JSValue setWidgetX(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        widget.setWidgetX(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetX(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        return jac::Value::from(ctx_, widget.widgetX()).loot().second;
    }

    static JSValue setWidgetY(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        widget.setWidgetY(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetY(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        return jac::Value::from(ctx_, widget.widgetY()).loot().second;
    }

    static JSValue setWidgetW(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        widget.setWidgetW(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetW(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        return jac::Value::from(ctx_, widget.widgetW()).loot().second;
    }

    static JSValue setWidgetH(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        widget.setWidgetH(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetH(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        return jac::Value::from(ctx_, widget.widgetH()).loot().second;
    }

    static JSValue setWidgetTab(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        widget.setWidgetTab(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetTab(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        return jac::Value::from(ctx_, widget.widgetTab()).loot().second;
    }

public:
    static JSValue css(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        auto val = widget.css(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return jac::Value::from(ctx_, val).loot().second;
    }

    static JSValue setCss(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = widgetOpaque<gridui::Widget>(thisVal);
        auto key = jac::ValueWeak(ctx_, argv[0]).to<std::string>();
        auto val = jac::ValueWeak(ctx_, argv[1]).to<std::string>();
        widget.setCss(key, val);
        return JS_UNDEFINED;
    }


    static void getProperty(const AtomString& name, qjsGetter* getter, qjsSetter *setter) {
        if(name == "uuid") {
            *getter = uuid;
            return;
        }
        if(name == "widgetX") {
            *getter = widgetX;
            *setter = setWidgetX;
            return;
        }
        if(name == "widgetY") {
            *getter = widgetY;
            *setter = setWidgetY;
            return;
        }
        if(name == "widgetW") {
            *getter = widgetW;
            *setter = setWidgetW;
            return;
        }
        if(name == "widgetH") {
            *getter = widgetH;
            *setter = setWidgetH;
            return;
        }
        if(name == "widgetTab") {
            *getter = widgetTab;
            *setter = setWidgetTab;
            return;
        }
    }
};

};
