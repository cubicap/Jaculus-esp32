#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetButton : public jac::ProtoBuilder::Opaque<gridui::Button>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setText(text);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.text();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setFontSize(fontSize);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.fontSize();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setBackground", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setBackground(background);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.background();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setAlign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string align) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setAlign(align);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("align", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.align();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setValign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string valign) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setValign(valign);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("valign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.valign();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setDisabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            w.setDisabled(disabled);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetButton::getOpaque(ctx, thisVal);
            return w.disabled();
        }), jac::PropFlags::Enumerable);
    }
};
