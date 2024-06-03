#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetCircle : public jac::ProtoBuilder::Opaque<gridui::Circle>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setFontSize(fontSize);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.fontSize();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setMin", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setMin(min);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.min();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setMax", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setMax(max);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.max();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setLineWidth", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float lineWidth) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setLineWidth(lineWidth);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("lineWidth", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.lineWidth();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setValueStart", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float valueStart) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setValueStart(valueStart);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("valueStart", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.valueStart();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setValue(value);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.value();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setShowValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool showValue) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            w.setShowValue(showValue);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("showValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCircle::getOpaque(ctx, thisVal);
            return w.showValue();
        }), jac::PropFlags::Enumerable);

    }
};
