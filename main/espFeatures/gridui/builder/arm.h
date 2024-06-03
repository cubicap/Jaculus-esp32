#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/arm.h"

struct GridUiBuilderArm : public jac::ProtoBuilder::Opaque<gridui::builder::Arm>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::builder::Arm* ptr) noexcept { }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);

        
        proto.defineProperty("info", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string info) {
            auto& builder = *GridUiBuilderArm::getOpaque(ctx, thisVal);
            builder.info(std::unique_ptr<rbjson::Object>(rbjson::parse((char*)info.c_str(), info.size())));
            return thisVal;
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("finish", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& builder = *GridUiBuilderArm::getOpaque(ctx, thisVal);
            return jac::Class<GridUiWidgetArm>::createInstance(ctx, new gridui::Arm(std::move(builder.finish())));
        }), jac::PropFlags::Enumerable);
    }
};
