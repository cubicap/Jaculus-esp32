#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiArmProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    

    return proto;
}

static jac::Object griduiBuilderArmProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    /*proto.set("info", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::unique_ptr<rbjson::Object> info) {
        auto& builder = *reinterpret_cast<gridui::builder::Arm*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.info(info);
        return thisVal;
    }));*/

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Arm*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Arm(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoArmWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiArmProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

