#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/circle.h"

struct GridUiBuilderCircle : public jac::ProtoBuilder::Opaque<gridui::builder::Circle>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Circle* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.min(min);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.max(max);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("lineWidth", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float lineWidth) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.lineWidth(lineWidth);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("valueStart", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float valueStart) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.valueStart(valueStart);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.value(value);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("showValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool showValue) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            builder.showValue(showValue);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderCircle::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetCircle>::createInstance(ctx, new gridui::Circle(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
