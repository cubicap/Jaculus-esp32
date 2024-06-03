#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/checkbox.h"

struct GridUiBuilderCheckbox : public jac::ProtoBuilder::Opaque<gridui::builder::Checkbox>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Checkbox* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& builder = *GridUiBuilderCheckbox::getOpaque(ctx, thisVal);
            builder.fontSize(fontSize);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("checked", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool checked) {
            auto& builder = *GridUiBuilderCheckbox::getOpaque(ctx, thisVal);
            builder.checked(checked);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderCheckbox::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& builder = *GridUiBuilderCheckbox::getOpaque(ctx, thisVal);
            builder.text(text);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderCheckbox::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetCheckbox>::createInstance(ctx, new gridui::Checkbox(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
