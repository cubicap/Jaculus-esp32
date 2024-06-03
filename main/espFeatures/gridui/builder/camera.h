#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/camera.h"

struct GridUiBuilderCamera : public jac::ProtoBuilder::Opaque<gridui::builder::Camera>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Camera* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("rotation", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float rotation) {
            auto& builder = *GridUiBuilderCamera::getOpaque(ctx, thisVal);
            builder.rotation(rotation);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("clip", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool clip) {
            auto& builder = *GridUiBuilderCamera::getOpaque(ctx, thisVal);
            builder.clip(clip);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderCamera::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetCamera>::createInstance(ctx, new gridui::Camera(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
