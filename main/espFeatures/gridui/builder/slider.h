#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/slider.h"

struct GridUiBuilderSlider : public jac::ProtoBuilder::Opaque<gridui::builder::Slider>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Slider* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.min(min);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.max(max);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.value(value);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("precision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float precision) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.precision(precision);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("showValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool showValue) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            builder.showValue(showValue);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderSlider::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetSlider>::createInstance(ctx, new gridui::Slider(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
