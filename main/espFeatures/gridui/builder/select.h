#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

#include "../widgets/_common.h"

namespace gridui_jac {

class SelectBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Select>(thisVal);
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue background(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Select>(thisVal);
        builder.background(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue disabled(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Select>(thisVal);
        builder.disabled(jac::ValueWeak(ctx_, argv[0]).to<bool>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue options(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Select>(thisVal);
        builder.options(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

    static JSValue selectedIndex(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = builderOpaque<gridui::builder::Select>(thisVal);
        builder.selectedIndex(jac::ValueWeak(ctx_, argv[0]).to<float>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static JSCFunction *getPropFunc(const AtomString& name) {
        using namespace gridui;

        if(name == "css") return builderCss<builder::Select>;
        if(name == "finish") return builderFinish<WidgetTypeId::Select, builder::Select, Select>;

        if(name == "color") return color;
        if(name == "background") return background;
        if(name == "disabled") return disabled;
        if(name == "options") return options;
        if(name == "selectedIndex") return selectedIndex;

        if(name == "onChanged") return &builderCallbackImpl<builder::Select, Select, &builder::Select::onChanged>;

        return nullptr;
    }
};

};
