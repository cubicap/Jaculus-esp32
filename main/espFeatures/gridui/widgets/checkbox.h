#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetCheckbox : public jac::ProtoBuilder::Opaque<gridui::Checkbox>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setFontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float fontSize) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            w.setFontSize(fontSize);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("fontSize", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            return w.fontSize();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setChecked", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool checked) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            w.setChecked(checked);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("checked", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            return w.checked();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setText", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string text) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            w.setText(text);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetCheckbox::getOpaque(ctx, thisVal);
            return w.text();
        }), jac::PropFlags::Enumerable);

    }
};
