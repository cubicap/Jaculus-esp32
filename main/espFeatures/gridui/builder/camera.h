#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/_common.h"

namespace gridui_jac {

class CameraBuilder {
    static JSValue rotation(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Camera>(thisVal);
        builder.rotation(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue clip(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Camera>(thisVal);
        builder.clip(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }


public:
    static JSCFunction *getPropFunc(const AtomString& name) {
        using namespace gridui;

        if(name == "css") return builderCss<builder::Camera>;
        if(name == "finish") return builderFinish<WidgetTypeId::Camera, builder::Camera, Camera>;

        if(name == "rotation") return rotation;
        if(name == "clip") return clip;

        return nullptr;
    }
};

};
