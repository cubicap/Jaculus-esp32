#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class CameraWidget {
    static JSValue setRotation(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setRotation(val.to<float>());
        return JS_UNDEFINED;
    }
    static JSValue rotation(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.rotation()).loot().second;
    }

    static JSValue setClip(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        widget.setClip(val.to<bool>());
        return JS_UNDEFINED;
    }
    static JSValue clip(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.clip()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("setRotation", jac::Value(ctx, JS_NewCFunction(ctx, setRotation, "setRotation", 0)));
        proto.set("rotation", jac::Value(ctx, JS_NewCFunction(ctx, rotation, "rotation", 0)));
        proto.set("setClip", jac::Value(ctx, JS_NewCFunction(ctx, setClip, "setClip", 0)));
        proto.set("clip", jac::Value(ctx, JS_NewCFunction(ctx, clip, "clip", 0)));
        return proto;
    }
};

};
