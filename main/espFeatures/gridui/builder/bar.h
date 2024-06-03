#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/bar.h"

struct GridUiBuilderBar : public jac::ProtoBuilder::Opaque<gridui::builder::Bar>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Bar* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            builder.min(min);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            builder.max(max);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            builder.value(value);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("showValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool showValue) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            builder.showValue(showValue);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderBar::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetBar>::createInstance(ctx, new gridui::Bar(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
