#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

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
    static JSCFunction *getPropFunc(const char *name) {
        using namespace gridui;

        if(strcmp(name, "css") == 0) return builderCss<builder::Select>;
        if(strcmp(name, "finish") == 0) return builderFinish<WidgetTypeId::Select, builder::Select, Select>;

        if(strcmp(name, "color") == 0) return color;
        if(strcmp(name, "background") == 0) return background;
        if(strcmp(name, "disabled") == 0) return disabled;
        if(strcmp(name, "options") == 0) return options;
        if(strcmp(name, "selectedIndex") == 0) return selectedIndex;

        if(strcmp(name, "onChanged") == 0) return &builderCallbackImpl<builder::Select, Select, &builder::Select::onChanged>;

        return nullptr;
    }
};

};
