#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/orientation.h"

struct GridUiBuilderOrientation : public jac::ProtoBuilder::Opaque<gridui::builder::Orientation>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Orientation* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& builder = *GridUiBuilderOrientation::getOpaque(ctx, thisVal);
            builder.color(color);
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderOrientation::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetOrientation>::createInstance(ctx, new gridui::Orientation(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
