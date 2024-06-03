#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/input.h"

struct GridUiBuilderInput : public jac::ProtoBuilder::Opaque<gridui::builder::Input>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Input* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& builder = *GridUiBuilderInput::getOpaque(ctx, thisVal);
            builder.text(text);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderInput::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("type", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string type) {
            auto& builder = *GridUiBuilderInput::getOpaque(ctx, thisVal);
            builder.type(type);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
            auto& builder = *GridUiBuilderInput::getOpaque(ctx, thisVal);
            builder.disabled(disabled);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderInput::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetInput>::createInstance(ctx, new gridui::Input(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
