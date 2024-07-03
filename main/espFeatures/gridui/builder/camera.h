#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class CameraBuilder {
    static JSValue rotation(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal, 1));
        builder.rotation(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue clip(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal, 1));
        builder.clip(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        using namespace gridui;

        auto proto = jac::Object::create(ctx);

        proto.set("rotation", jac::Value(ctx, JS_NewCFunction(ctx, rotation, "rotation", 1)));
        proto.set("clip", jac::Value(ctx, JS_NewCFunction(ctx, clip, "clip", 1)));

        return proto;
    }
};

};
