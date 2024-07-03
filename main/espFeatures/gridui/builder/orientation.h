#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class OrientationBuilder {
    static JSValue color(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Orientation*>(JS_GetOpaque(thisVal, 1));
        builder.color(jac::ValueWeak(ctx_, argv[0]).to<std::string>());
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        using namespace gridui;

        auto proto = jac::Object::create(ctx);

        proto.set("color", jac::Value(ctx, JS_NewCFunction(ctx, color, "color", 1)));

        defineBuilderCallback<builder::Orientation, Orientation, &builder::Orientation::onPositionChanged>(ctx, proto, "onPositionChanged");

        return proto;
    }
};

};
