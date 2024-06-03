#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/text.h"

struct GridUiBuilderText : public jac::ProtoBuilder::Opaque<gridui::builder::Text>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Text* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.text(text);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.background(background);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("align", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string align) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.align(align);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("valign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string valign) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.valign(valign);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("prefix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string prefix) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.prefix(prefix);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("suffix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string suffix) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            builder.suffix(suffix);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderText::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetText>::createInstance(ctx, new gridui::Text(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
