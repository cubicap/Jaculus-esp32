#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class CircleWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setFontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setFontSize(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue fontSize(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.fontSize()).loot().second;
    }

    static JSValue setMin(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setMin(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue min(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.min()).loot().second;
    }

    static JSValue setMax(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setMax(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue max(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.max()).loot().second;
    }

    static JSValue setLineWidth(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setLineWidth(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue lineWidth(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.lineWidth()).loot().second;
    }

    static JSValue setValueStart(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setValueStart(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue valueStart(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.valueStart()).loot().second;
    }

    static JSValue setValue(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setValue(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue value(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.value()).loot().second;
    }

    static JSValue setShowValue(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setShowValue(val.to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue showValue(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Circle*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.showValue()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("setFontSize", jac::Value(ctx, JS_NewCFunction(ctx, setFontSize, "setFontSize", 0)));
        proto.set("fontSize", jac::Value(ctx, JS_NewCFunction(ctx, fontSize, "fontSize", 0)));
        proto.set("setMin", jac::Value(ctx, JS_NewCFunction(ctx, setMin, "setMin", 0)));
        proto.set("min", jac::Value(ctx, JS_NewCFunction(ctx, min, "min", 0)));
        proto.set("setMax", jac::Value(ctx, JS_NewCFunction(ctx, setMax, "setMax", 0)));
        proto.set("max", jac::Value(ctx, JS_NewCFunction(ctx, max, "max", 0)));
        proto.set("setLineWidth", jac::Value(ctx, JS_NewCFunction(ctx, setLineWidth, "setLineWidth", 0)));
        proto.set("lineWidth", jac::Value(ctx, JS_NewCFunction(ctx, lineWidth, "lineWidth", 0)));
        proto.set("setValueStart", jac::Value(ctx, JS_NewCFunction(ctx, setValueStart, "setValueStart", 0)));
        proto.set("valueStart", jac::Value(ctx, JS_NewCFunction(ctx, valueStart, "valueStart", 0)));
        proto.set("setValue", jac::Value(ctx, JS_NewCFunction(ctx, setValue, "setValue", 0)));
        proto.set("value", jac::Value(ctx, JS_NewCFunction(ctx, value, "value", 0)));
        proto.set("setShowValue", jac::Value(ctx, JS_NewCFunction(ctx, setShowValue, "setShowValue", 0)));
        proto.set("showValue", jac::Value(ctx, JS_NewCFunction(ctx, showValue, "showValue", 0)));
        return proto;
    }
};

};
