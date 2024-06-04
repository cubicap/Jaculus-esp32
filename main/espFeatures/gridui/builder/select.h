#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiSelectProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setBackground", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setBackground(background);
    }));
    proto.set("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.background();
    }));
    proto.set("setDisabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setDisabled(disabled);
    }));
    proto.set("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.disabled();
    }));
    proto.set("setOptions", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string options) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setOptions(options);
    }));
    proto.set("options", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.options();
    }));
    proto.set("setSelectedIndex", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float selectedIndex) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setSelectedIndex(selectedIndex);
    }));
    proto.set("selectedIndex", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.selectedIndex();
    }));

    return proto;
}

static jac::Object griduiBuilderSelectProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
        auto& builder = *reinterpret_cast<gridui::builder::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.background(background);
        return thisVal;
    }));
    proto.set("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
        auto& builder = *reinterpret_cast<gridui::builder::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.disabled(disabled);
        return thisVal;
    }));
    proto.set("options", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string options) {
        auto& builder = *reinterpret_cast<gridui::builder::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.options(options);
        return thisVal;
    }));
    proto.set("selectedIndex", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float selectedIndex) {
        auto& builder = *reinterpret_cast<gridui::builder::Select*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.selectedIndex(selectedIndex);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Select*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Select(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoSelectWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiSelectProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

