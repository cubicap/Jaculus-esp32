#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/_common.h"

namespace gridui_jac {

class ArmBuilder {
    static JSValue info(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Arm>(thisVal);
        auto json = jac::ValueWeak(ctx_, argv[0]).to<std::string>();
        builder.info(std::unique_ptr<rbjson::Object>(rbjson::parse((char*)json.c_str(), json.size())));
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const AtomString& name) {
        using namespace gridui;

        if(name == "css") return builderCss<builder::Arm>;
        if(name == "finish") return builderFinish<WidgetTypeId::Arm, builder::Arm, Arm>;

        if(name == "info") return info;

        if(name == "onGrab") return &builderCallbackImpl<builder::Arm, Arm, &builder::Arm::onGrab>;
        if(name == "onPositionChanged") return &builderCallbackImpl<builder::Arm, Arm, &builder::Arm::onPositionChanged>;

        return nullptr;
    }
};

};
