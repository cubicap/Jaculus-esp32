#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetArm : public jac::ProtoBuilder::Opaque<gridui::Arm>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
    }
};
