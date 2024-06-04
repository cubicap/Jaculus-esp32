#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiJoystickProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setKeys", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string keys) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setKeys(keys);
    }));
    proto.set("keys", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.keys();
    }));
    proto.set("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setText(text);
    }));
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.text();
    }));

    return proto;
}

static jac::Object griduiBuilderJoystickProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("keys", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string keys) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.keys(keys);
        return thisVal;
    }));
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.text(text);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Joystick*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Joystick(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoJoystickWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiJoystickProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

