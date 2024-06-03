#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/spinedit.h"

struct GridUiBuilderSpinEdit : public jac::ProtoBuilder::Opaque<gridui::builder::SpinEdit>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::SpinEdit* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderSpinEdit::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderSpinEdit::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
            auto& builder = *GridUiBuilderSpinEdit::getOpaque(ctx, thisVal);
            builder.value(value);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("step", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float step) {
            auto& builder = *GridUiBuilderSpinEdit::getOpaque(ctx, thisVal);
            builder.step(step);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("precision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float precision) {
            auto& builder = *GridUiBuilderSpinEdit::getOpaque(ctx, thisVal);
            builder.precision(precision);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderSpinEdit::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetSpinEdit>::createInstance(ctx, new gridui::SpinEdit(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
