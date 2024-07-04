#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>
#include "./_common.h"

namespace gridui_jac {

class ArmWidget {
    static JSValue x(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Arm>(thisVal);
        return jac::Value::from(ctx_, widget.x()).loot().second;
    }

    static JSValue y(JSContext* ctx_, JSValueConst thisVal) {
        auto& widget = widgetOpaque<gridui::Arm>(thisVal);
        return jac::Value::from(ctx_, widget.y()).loot().second;
    }

public:
    static void getProperty(const char *name, qjsGetter* getter, qjsSetter *setter) {
        if(strcmp(name, "x") == 0) {
            *getter = x;
            return;
        }
        if(strcmp(name, "y") == 0) {
            *getter = y;
            return;
        }
    }
};

};
