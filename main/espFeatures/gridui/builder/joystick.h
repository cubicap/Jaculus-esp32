#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/joystick.h"

struct GridUiBuilderJoystick : public jac::ProtoBuilder::Opaque<gridui::builder::Joystick>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Joystick* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderJoystick::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("keys", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string keys) {
            auto& builder = *GridUiBuilderJoystick::getOpaque(ctx, thisVal);
            builder.keys(keys);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& builder = *GridUiBuilderJoystick::getOpaque(ctx, thisVal);
            builder.text(text);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderJoystick::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetJoystick>::createInstance(ctx, new gridui::Joystick(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
