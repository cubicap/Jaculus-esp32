#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

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
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Camera>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Camera, builder::Camera, Camera>;

        if(strcmp(name, "rotation") == 0) return rotation;
        if(strcmp(name, "clip") == 0) return clip;

        return nullptr;
    }
};

};
