#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class ArmBuilder {
    static JSValue info(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Arm>(thisVal);
        auto json = jac::ValueWeak(ctx_, argv[0]).to<std::string>();
        builder.info(std::unique_ptr<rbjson::Object>(rbjson::parse((char*)json.c_str(), json.size())));
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Arm>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Arm, builder::Arm, Arm>;

        if(strcmp(name, "info") == 0) return info;

        if(strcmp(name, "onGrab") == 0) return &builderCallbackImpl<builder::Arm, Arm, &builder::Arm::onGrab>;
        if(strcmp(name, "onPositionChanged") == 0) return &builderCallbackImpl<builder::Arm, Arm, &builder::Arm::onPositionChanged>;

        return nullptr;
    }
};

};
