#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiBarProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setFontSize(fontSize);
    }));
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.fontSize();
    }));
    proto.set("setMin", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setMin(min);
    }));
    proto.set("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.min();
    }));
    proto.set("setMax", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setMax(max);
    }));
    proto.set("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.max();
    }));
    proto.set("setValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setValue(value);
    }));
    proto.set("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.value();
    }));
    proto.set("setShowValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool showValue) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setShowValue(showValue);
    }));
    proto.set("showValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.showValue();
    }));

    return proto;
}

static jac::Object griduiBuilderBarProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.fontSize(fontSize);
        return thisVal;
    }));
    proto.set("min", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float min) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.min(min);
        return thisVal;
    }));
    proto.set("max", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float max) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.max(max);
        return thisVal;
    }));
    proto.set("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.value(value);
        return thisVal;
    }));
    proto.set("showValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool showValue) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.showValue(showValue);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Bar*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Bar(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoBarWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiBarProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

