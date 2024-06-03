#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetCamera : public jac::ProtoBuilder::Opaque<gridui::Camera>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setRotation", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float rotation) {
            auto& w = *GridUiWidgetCamera::getOpaque(ctx, thisVal);
            w.setRotation(rotation);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("rotation", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCamera::getOpaque(ctx, thisVal);
            return w.rotation();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setClip", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool clip) {
            auto& w = *GridUiWidgetCamera::getOpaque(ctx, thisVal);
            w.setClip(clip);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("clip", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCamera::getOpaque(ctx, thisVal);
            return w.clip();
        }), jac::PropFlags::Enumerable);

    }
};
