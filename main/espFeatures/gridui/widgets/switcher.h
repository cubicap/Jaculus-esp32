#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetSwitcher : public jac::ProtoBuilder::Opaque<gridui::Switcher>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            w.setFontSize(fontSize);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            return w.fontSize();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            return w.value();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setMin", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            w.setMin(min);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            return w.min();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setMax", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            w.setMax(max);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSwitcher::getOpaque(ctx, thisVal);
            return w.max();
        }), jac::PropFlags::Enumerable);

    }
};
