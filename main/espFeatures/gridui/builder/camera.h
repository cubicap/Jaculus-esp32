#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class CameraBuilder {
    static JSValue rotation(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.rotation(val.to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue clip(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        builder.clip(val.to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("rotation", jac::Value(ctx, JS_NewCFunction(ctx, rotation, "rotation", 0)));
        proto.set("clip", jac::Value(ctx, JS_NewCFunction(ctx, clip, "clip", 0)));
        return proto;
    }
};

};
