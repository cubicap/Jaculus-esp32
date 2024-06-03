#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetLed : public jac::ProtoBuilder::Opaque<gridui::Led>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetLed::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetLed::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setOn", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool on) {
            auto& w = *GridUiWidgetLed::getOpaque(ctx, thisVal);
            w.setOn(on);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("on", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetLed::getOpaque(ctx, thisVal);
            return w.on();
        }), jac::PropFlags::Enumerable);

    }
};
