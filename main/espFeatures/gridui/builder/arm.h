#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "../widgets/arm.h"

namespace gridui_jac {

class ArmBuilder {
    static JSValue info(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Arm*>(JS_GetOpaque(thisVal, 1));
        auto json = jac::ValueWeak(ctx_, argv[0]).to<std::string>();
        builder.info(std::unique_ptr<rbjson::Object>(rbjson::parse((char*)json.c_str(), json.size())));
        return JS_DupValue(ctx_, thisVal);
    }


public:
    static jac::Object proto(jac::ContextRef ctx) {
        using namespace gridui;

        auto proto = jac::Object::create(ctx);

        proto.set("info", jac::Value(ctx, JS_NewCFunction(ctx, info, "info", 1)));

        defineBuilderCallback<builder::Arm, Arm, &builder::Arm::onGrab>(ctx, proto, "onGrab");
        defineBuilderCallback<builder::Arm, Arm, &builder::Arm::onPositionChanged>(ctx, proto, "onPositionChanged");

        return proto;
    }
};

};
