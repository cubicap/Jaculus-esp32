#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

static jac::Object griduiTextProto(jac::ContextRef ctx) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setText(text);
    }));
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.text();
    }));
    proto.set("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setFontSize(fontSize);
    }));
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.fontSize();
    }));
    proto.set("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setColor(color);
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.color();
    }));
    proto.set("setBackground", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setBackground(background);
    }));
    proto.set("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.background();
    }));
    proto.set("setAlign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string align) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setAlign(align);
    }));
    proto.set("align", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.align();
    }));
    proto.set("setValign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string valign) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setValign(valign);
    }));
    proto.set("valign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.valign();
    }));
    proto.set("setPrefix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string prefix) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setPrefix(prefix);
    }));
    proto.set("prefix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.prefix();
    }));
    proto.set("setSuffix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string suffix) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        widget.setSuffix(suffix);
    }));
    proto.set("suffix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& widget = *reinterpret_cast<gridui::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        return widget.suffix();
    }));

    return proto;
}

static jac::Object griduiBuilderTextProto(jac::ContextRef ctx, std::map<intptr_t, jac::Object>& _protoCache) {
    jac::FunctionFactory ff(ctx);

    auto proto = jac::Object::create(ctx);
    
    proto.set("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.text(text);
        return thisVal;
    }));
    proto.set("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.fontSize(fontSize);
        return thisVal;
    }));
    proto.set("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.color(color);
        return thisVal;
    }));
    proto.set("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.background(background);
        return thisVal;
    }));
    proto.set("align", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string align) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.align(align);
        return thisVal;
    }));
    proto.set("valign", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string valign) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.valign(valign);
        return thisVal;
    }));
    proto.set("prefix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string prefix) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.prefix(prefix);
        return thisVal;
    }));
    proto.set("suffix", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string suffix) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));
        builder.suffix(suffix);
        return thisVal;
    }));

    proto.set("finish", ff.newFunctionThis([&_protoCache](jac::ContextRef ctx, jac::ValueWeak thisVal) {
        auto& builder = *reinterpret_cast<gridui::builder::Text*>(JS_GetOpaque(thisVal.getVal(), 1));

        auto widget = new gridui::Text(std::move(builder.finish()));

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        static const char *protoName = "protoTextWidget";
        auto protoKey = (intptr_t)protoName;

        auto itr = _protoCache.find(protoKey);
        if(itr == _protoCache.end()) {
            auto proto = griduiTextProto(ctx);
            obj.setPrototype(proto);
            _protoCache.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }

        return obj;
    }));
    return proto;
}

