#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class BaseWidget {
    static JSValue uuid(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.uuid()).loot().second;
    }

    static JSValue setWidgetX(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        widget.setWidgetX(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetX(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.widgetX()).loot().second;
    }

    static JSValue setWidgetY(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        widget.setWidgetY(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetY(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.widgetY()).loot().second;
    }

    static JSValue setWidgetW(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        widget.setWidgetW(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetW(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.widgetW()).loot().second;
    }

    static JSValue setWidgetH(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        widget.setWidgetH(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetH(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.widgetH()).loot().second;
    }

    static JSValue setWidgetTab(JSContext* ctx_, JSValueConst thisVal, JSValueConst val) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        widget.setWidgetTab(jac::ValueWeak(ctx_, val).to<float>());
        return JS_UNDEFINED;
    }
    static JSValue widgetTab(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.widgetTab()).loot().second;
    }

    static JSValue css(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        auto val = widget.css(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return jac::Value::from(ctx_, val).loot().second;
    }
    static JSValue setCss(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Widget*>(JS_GetOpaque(thisVal, 1));
        auto key = jac::ValueWeak(ctx_, argv[0]).to<std::string>();
        auto val = jac::ValueWeak(ctx_, argv[1]).to<std::string>();
        widget.setCss(key, val);
        return JS_UNDEFINED;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetPropertyReadOnly(ctx, proto, "uuid", uuid);
        defineWidgetProperty(ctx, proto, "widgetX", "setWidgetX", widgetX, setWidgetX);
        defineWidgetProperty(ctx, proto, "widgetY", "setWidgetY", widgetY, setWidgetY);
        defineWidgetProperty(ctx, proto, "widgetW", "setWidgetW", widgetW, setWidgetW);
        defineWidgetProperty(ctx, proto, "widgetH", "setWidgetH", widgetH, setWidgetH);
        defineWidgetProperty(ctx, proto, "widgetTab", "setWidgetTab", widgetTab, setWidgetTab);

        proto.set("css", jac::Value(ctx, JS_NewCFunction(ctx, css, "css", 1)));
        proto.set("setCss", jac::Value(ctx, JS_NewCFunction(ctx, setCss, "setCss", 2)));
        return proto;
    }
};

};
