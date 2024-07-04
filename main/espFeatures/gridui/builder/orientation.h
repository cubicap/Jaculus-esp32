#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class OrientationBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Orientation>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Orientation>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Orientation, builder::Orientation, Orientation>;

        if(strcmp(name, "color") == 0) return color;

        if(strcmp(name, "onPositionChanged") == 0) return &builderCallbackImpl<builder::Orientation, Orientation, &builder::Orientation::onPositionChanged>;

        return nullptr;
    }
};

};
