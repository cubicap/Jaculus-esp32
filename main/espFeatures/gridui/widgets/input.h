#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetInput : public jac::ProtoBuilder::Opaque<gridui::Input>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            w.setText(text);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            return w.text();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setType", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string type) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            w.setType(type);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("type", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            return w.type();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setDisabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            w.setDisabled(disabled);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetInput::getOpaque(ctx, thisVal);
            return w.disabled();
        }), jac::PropFlags::Enumerable);

    }
};
