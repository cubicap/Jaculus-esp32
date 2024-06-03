#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetSpinEdit : public jac::ProtoBuilder::Opaque<gridui::SpinEdit>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            w.setFontSize(fontSize);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            return w.fontSize();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            w.setValue(value);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            return w.value();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setStep", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float step) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            w.setStep(step);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("step", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            return w.step();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setPrecision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float precision) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            w.setPrecision(precision);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("precision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSpinEdit::getOpaque(ctx, thisVal);
            return w.precision();
        }), jac::PropFlags::Enumerable);

    }
};
