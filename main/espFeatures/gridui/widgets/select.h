#pragma once

#include <jac/machine/functionFactory.h>
#include <gridui.h>

struct GridUiWidgetSelect : public jac::ProtoBuilder::Opaque<gridui::Select>, public jac::ProtoBuilder::Properties {
    static void addProperties(JSContext* ctx, jac::Object proto) {
        jac::FunctionFactory ff(ctx);
        
        proto.defineProperty("setColor", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string color) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            w.setColor(color);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("color", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            return w.color();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setBackground", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string background) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            w.setBackground(background);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("background", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            return w.background();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setDisabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, bool disabled) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            w.setDisabled(disabled);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("disabled", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            return w.disabled();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setOptions", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, std::string options) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            w.setOptions(options);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("options", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            return w.options();
        }), jac::PropFlags::Enumerable);

        proto.defineProperty("setSelectedIndex", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal, float selectedIndex) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            w.setSelectedIndex(selectedIndex);
        }), jac::PropFlags::Enumerable);
        proto.defineProperty("selectedIndex", ff.newFunctionThis([](jac::ContextRef ctx, jac::ValueWeak thisVal) {
            auto& w = *GridUiWidgetSelect::getOpaque(ctx, thisVal);
            return w.selectedIndex();
        }), jac::PropFlags::Enumerable);

    }
};
