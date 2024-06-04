#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiCameraProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setRotation", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float rotation) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setRotation(rotation);
    }));
    proto.set("rotation", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.rotation();
    }));
    proto.set("setClip", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool clip) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setClip(clip);
    }));
    proto.set("clip", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.clip();
    }));

    return proto;
}

static jac::Object griduiBuilderCameraProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("rotation", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float rotation) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.rotation(rotation);
        return thisVal;
    }));
    proto.set("clip", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool clip) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.clip(clip);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Camera*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Camera(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoCameraWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiCameraProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

