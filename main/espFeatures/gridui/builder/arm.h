#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

namespace gridui_jac {

class ArmBuilder {
    static JSValue info(JSContext* ctx_, JSValueConst thisVal, int argc, JSValueConst* argv) {
        auto& builder = *reinterpret_cast<gridui::builder::Arm*>(JS_GetOpaque(thisVal, 1));
        if(argc < 1) throw jac::Exception::create(jac::Exception::Type::TypeError, "1 argument expected");
        auto val = jac::ValueWeak(jac::ContextRef(ctx_), argv[0]);
        auto json = val.to<std::string>();
        builder.info(std::unique_ptr<rbjson::Object>(rbjson::parse((char*)json.c_str(), json.size())));
        return JS_DupValue(ctx_, thisVal);
    }

public:
    static jac::Object proto(jac::ContextRef ctx) {
        auto proto = jac::Object::create(ctx);
        proto.set("info", jac::Value(ctx, JS_NewCFunction(ctx, info, "info", 0)));
        return proto;
    }
};

};
