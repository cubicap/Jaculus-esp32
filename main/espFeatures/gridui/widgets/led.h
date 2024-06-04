#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class LedWidget {
    static JSValue setColor(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setColor(val.to<std::string>());
        return JS_UNDEFINED;
    }
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.color()).loot().second;
    }

    static JSValue setOn(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setOn(val.to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue on(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.on()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setColor", jac::Value(ctx, JS_NewCFunction(ctx, setColor, "setColor", 0)));
        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 0)));
        proto.set("setOn", jac::Value(ctx, JS_NewCFunction(ctx, setOn, "setOn", 0)));
        proto.set("on", jac::Value(ctx, JS_NewCFunction(ctx, on, "on", 0)));
        return proto;
    }
};

};
