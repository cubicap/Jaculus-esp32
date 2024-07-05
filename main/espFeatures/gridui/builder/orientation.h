#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/_common.h"

namespace gridui_jac {

class OrientationBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Orientation>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const AtomString& name) {
        using namespace gridui;

        if(name == "css") return builderCss<builder::Orientation>;
        if(name == "finish") return builderFinish<WidgetTypeId::Orientation, builder::Orientation, Orientation>;

        if(name == "color") return color;

        if(name == "onPositionChanged") return &builderCallbackImpl<builder::Orientation, Orientation, &builder::Orientation::onPositionChanged>;

        return nullptr;
    }
};

};
