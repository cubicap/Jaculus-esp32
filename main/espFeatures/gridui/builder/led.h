#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/led.h"

struct GridUiBuilderLed : public jac::ProtoBuilder::Opaque<gridui::builder::Led>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Led* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderLed::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("on", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool on) {
            auto& builder = *GridUiBuilderLed::getOpaque(ctx, thisVal);
            builder.on(on);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderLed::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetLed>::createInstance(ctx, new gridui::Led(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
