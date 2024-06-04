#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiCheckboxProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setFontSize(fontSize);
    }));
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.fontSize();
    }));
    proto.set("setChecked", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool checked) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setChecked(checked);
    }));
    proto.set("checked", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.checked();
    }));
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setText(text);
    }));
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.text();
    }));

    return proto;
}

static jac::Object griduiBuilderCheckboxProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& builder = *reinterpret_cast<gridui::builder::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.fontSize(fontSize);
        return thisVal;
    }));
    proto.set("checked", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool checked) {
        auto& builder = *reinterpret_cast<gridui::builder::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.checked(checked);
        return thisVal;
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& builder = *reinterpret_cast<gridui::builder::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.text(text);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Checkbox*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Checkbox(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoCheckboxWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiCheckboxProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

