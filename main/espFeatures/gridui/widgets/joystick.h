#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetJoystick : public jac::ProtoBuilder::Opaque<gridui::Joystick>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetJoystick::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetJoystick::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setKeys", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string keys) {
            auto& w = *GridUiWidgetJoystick::getOpaque(ctx, thisVal);
            w.setKeys(keys);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("keys", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetJoystick::getOpaque(ctx, thisVal);
            return w.keys();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& w = *GridUiWidgetJoystick::getOpaque(ctx, thisVal);
            w.setText(text);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetJoystick::getOpaque(ctx, thisVal);
            return w.text();
        }), jac::PropFlags::Enumerable);

    }
};
