#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/switcher.h"

struct GridUiBuilderSwitcher : public jac::ProtoBuilder::Opaque<gridui::builder::Switcher>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Switcher* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderSwitcher::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderSwitcher::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
            auto& builder = *GridUiBuilderSwitcher::getOpaque(ctx, thisVal);
            builder.min(min);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
            auto& builder = *GridUiBuilderSwitcher::getOpaque(ctx, thisVal);
            builder.max(max);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderSwitcher::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetSwitcher>::createInstance(ctx, new gridui::Switcher(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
