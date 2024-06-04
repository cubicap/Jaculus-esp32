#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiSpinEditProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setFontSize(fontSize);
    }));
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.fontSize();
    }));
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setValue", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setValue(value);
    }));
    proto.set("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.value();
    }));
    proto.set("setStep", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float step) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setStep(step);
    }));
    proto.set("step", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.step();
    }));
    proto.set("setPrecision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float precision) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setPrecision(precision);
    }));
    proto.set("precision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.precision();
    }));

    return proto;
}

static jac::Object griduiBuilderSpinEditProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.fontSize(fontSize);
        return thisVal;
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("value", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float value) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.value(value);
        return thisVal;
    }));
    proto.set("step", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float step) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.step(step);
        return thisVal;
    }));
    proto.set("precision", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float precision) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.precision(precision);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::SpinEdit*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::SpinEdit(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoSpinEditWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiSpinEditProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

