#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/button.h"

struct GridUiBuilderButton : public jac::ProtoBuilder::Opaque<gridui::builder::Button>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Button* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.text(text);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.background(background);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("align", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string align) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.align(align);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("valign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string valign) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.valign(valign);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            builder.disabled(disabled);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderButton::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetButton>::createInstance(ctx, new gridui::Button(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
