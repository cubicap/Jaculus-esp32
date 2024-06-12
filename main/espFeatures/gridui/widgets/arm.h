#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class ArmWidget {
    static JSValue x(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Arm*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.x()).loot().second;
    }

    static JSValue y(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = *reinterpret_cast<gridui::Arm*>(JS_GetOpaque(thisVal, 1));
        return jac::Value::from(ctx_, widget.y()).loot().second;
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        defineWidgetPropertyReadOnly(ctx, proto, "x", x);
        defineWidgetPropertyReadOnly(ctx, proto, "y", y);
        return proto;
    }
};

};
