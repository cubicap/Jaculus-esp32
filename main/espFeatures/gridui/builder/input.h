#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiInputProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setText(text);
    }));
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.text();
    }));
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setType", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string type) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setType(type);
    }));
    proto.set("type", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.type();
    }));
    proto.set("setDisabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setDisabled(disabled);
    }));
    proto.set("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.disabled();
    }));

    return proto;
}

static jac::Object griduiBuilderInputProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& builder = *reinterpret_cast<gridui::builder::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.text(text);
        return thisVal;
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("type", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string type) {
        auto& builder = *reinterpret_cast<gridui::builder::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.type(type);
        return thisVal;
    }));
    proto.set("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
        auto& builder = *reinterpret_cast<gridui::builder::Input*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.disabled(disabled);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Input*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Input(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoInputWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiInputProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

