#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiLedProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setOn", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool on) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setOn(on);
    }));
    proto.set("on", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Led*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.on();
    }));

    return proto;
}

static jac::Object griduiBuilderLedProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Led*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("on", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool on) {
        auto& builder = *reinterpret_cast<gridui::builder::Led*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.on(on);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Led*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Led(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoLedWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiLedProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

