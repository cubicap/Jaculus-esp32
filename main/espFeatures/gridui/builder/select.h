#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/select.h"

struct GridUiBuilderSelect : public jac::ProtoBuilder::Opaque<gridui::builder::Select>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Select* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderSelect::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
            auto& builder = *GridUiBuilderSelect::getOpaque(ctx, thisVal);
            builder.background(background);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
            auto& builder = *GridUiBuilderSelect::getOpaque(ctx, thisVal);
            builder.disabled(disabled);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("options", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string options) {
            auto& builder = *GridUiBuilderSelect::getOpaque(ctx, thisVal);
            builder.options(options);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("selectedIndex", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float selectedIndex) {
            auto& builder = *GridUiBuilderSelect::getOpaque(ctx, thisVal);
            builder.selectedIndex(selectedIndex);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderSelect::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetSelect>::createInstance(ctx, new gridui::Select(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
