#include <string.h>

#include "gridui.h"
#include "_common.h"

#include "../builder/arm.h"
#include "../builder/bar.h"
#include "../builder/button.h"
#include "../builder/camera.h"
#include "../builder/checkbox.h"
#include "../builder/circle.h"
#include "../builder/input.h"
#include "../builder/joystick.h"
#include "../builder/led.h"
#include "../builder/orientation.h"
#include "../builder/select.h"
#include "../builder/slider.h"
#include "../builder/spinedit.h"
#include "../builder/switcher.h"
#include "../builder/text.h"

#include "./base.h"

#include "./arm.h"
#include "./bar.h"
#include "./button.h"
#include "./camera.h"
#include "./checkbox.h"
#include "./circle.h"
#include "./input.h"
#include "./joystick.h"
#include "./led.h"
#include "./orientation.h"
#include "./select.h"
#include "./slider.h"
#include "./spinedit.h"
#include "./switcher.h"
#include "./text.h"


namespace gridui_jac {

JSClassID GridUiContext::_builderClassId = 0;
JSClassID GridUiContext::_widgetClassId = 0;

JSClassExoticMethods GridUiContext::builderExotic = {
    .get_own_property = GridUiContext::builderGetOwnProperty,
    .get_own_property_names = nullptr,
    .delete_property = nullptr,
    .define_own_property = nullptr,
    .has_property = nullptr,
    .get_property = nullptr,
    .set_property = nullptr,
};

JSClassExoticMethods GridUiContext::widgetExotic = {
    .get_own_property = GridUiContext::widgetGetOwnProperty,
    .get_own_property_names = nullptr,
    .delete_property = nullptr,
    .define_own_property = nullptr,
    .has_property = nullptr,
    .get_property = nullptr,
    .set_property = nullptr,
};

void GridUiContext::createClasses(jac::ContextRef context) {
    JSRuntime* rt = JS_GetRuntime(context.get());

    if(_builderClassId != 0 && JS_IsRegisteredClass(rt, _builderClassId)) {
        return;
    }

    auto proto = JS_NewObject(context.get());

    if(_builderClassId == 0) {
        JS_NewClassID(&_builderClassId);
    }

    JSClassDef builderClassDef = {
        .class_name = "GridUiWidgetBuilder",
        .finalizer = nullptr,
        .gc_mark = nullptr,
        .call = nullptr,
        .exotic = &builderExotic,
    };
    JS_NewClass(rt, _builderClassId, &builderClassDef);
    JS_SetClassProto(context.get(), _builderClassId, JS_DupValue(context.get(), proto));

    if(_widgetClassId == 0) {
        JS_NewClassID(&_widgetClassId);
    }
    JSClassDef widgetClassDef = {
        .class_name = "GridUiWidget",
        .finalizer = nullptr,
        .gc_mark = nullptr,
        .call = nullptr,
        .exotic = &widgetExotic,
    };
    JS_NewClass(rt, _widgetClassId, &widgetClassDef);
    JS_SetClassProto(context.get(), _widgetClassId, JS_DupValue(context.get(), proto));

    JS_FreeValue(context.get(), proto);
}

int GridUiContext::builderGetOwnProperty(JSContext *ctx, JSPropertyDescriptor *desc, JSValueConst obj, JSAtom prop) {
    auto *base = reinterpret_cast<gridui::builder::Widget*>(JS_GetOpaque(obj, _builderClassId));
    if(!base) {
        return 0;
    }

    JSCFunction *getFunc = nullptr;

    const char *wName = base->widgetTypeName();
    const int wNameLen = strlen(wName);

    AtomString propName(ctx, prop);

    if(literalEqual(wName, wNameLen, "Arm")) getFunc = ArmBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Bar")) getFunc = BarBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Button")) getFunc = ButtonBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Camera")) getFunc = CameraBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Checkbox")) getFunc = CheckboxBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Circle")) getFunc = CircleBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Input")) getFunc = InputBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Joystick")) getFunc = JoystickBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Led")) getFunc = LedBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Orientation")) getFunc = OrientationBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Select")) getFunc = SelectBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Slider")) getFunc = SliderBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "SpinEdit")) getFunc = SpinEditBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Switcher")) getFunc = SwitcherBuilder::getPropFunc(propName);
    else if(literalEqual(wName, wNameLen, "Text")) getFunc = TextBuilder::getPropFunc(propName);
    else {
        throw jac::Exception::create(jac::Exception::Type::TypeError, std::string("Invalid type received: ") + wName);
    }

    if(getFunc) {
        int argc = 1;
        if(propName == "css") {
            argc = 2;
        }

        desc->value = JS_NewCFunction(ctx, getFunc, propName.c_str(), argc);
        desc->flags = JS_PROP_ENUMERABLE;
        desc->getter = JS_UNDEFINED;
        desc->setter = JS_UNDEFINED;
    }

    return getFunc ? 1 : 0;
}

int GridUiContext::widgetGetOwnProperty(JSContext *ctx, JSPropertyDescriptor *desc, JSValueConst obj, JSAtom prop) {
    auto *base = reinterpret_cast<gridui::Widget*>(JS_GetOpaque(obj, _widgetClassId));
    if(!base) {
        return 0;
    }

    AtomString propName(ctx, prop);

    qjsGetter getter = nullptr;
    qjsSetter setter = nullptr;

    auto typeIdValue = JS_GetPropertyUint32(ctx, obj, 0);
    auto typeId = static_cast<WidgetTypeId>(JS_VALUE_GET_INT(typeIdValue));
    JS_FreeValue(ctx, typeIdValue);

    switch(typeId) {
        case WidgetTypeId::Base: break;
        case WidgetTypeId::Arm: ArmWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Bar: BarWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Button: ButtonWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Camera: CameraWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Checkbox: CheckboxWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Circle: CircleWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Input: InputWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Joystick: JoystickWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Led: LedWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Orientation: OrientationWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Select: SelectWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Slider: SliderWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::SpinEdit: SpinEditWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Switcher: SwitcherWidget::getProperty(propName, &getter, &setter); break;
        case WidgetTypeId::Text: TextWidget::getProperty(propName, &getter, &setter); break;
    }

    if(!getter && !setter) {
        if(propName == "css") {
            desc->value = JS_NewCFunction(ctx, WidgetWidget::css, propName.c_str(), 1);
            desc->flags = JS_PROP_ENUMERABLE;
            desc->getter = JS_UNDEFINED;
            desc->setter = JS_UNDEFINED;
            return 1;
        } else if(propName == "setCss") {
            desc->value = JS_NewCFunction(ctx, WidgetWidget::setCss, propName.c_str(), 2);
            desc->flags = JS_PROP_ENUMERABLE;
            desc->getter = JS_UNDEFINED;
            desc->setter = JS_UNDEFINED;
            return 1;
        } else {
            WidgetWidget::getProperty(propName, &getter, &setter);
        }
    }

    if(!getter && !setter) {
        return 0;
    }

    desc->value = JS_UNDEFINED;
    desc->flags = JS_PROP_GETSET;

    if(getter) {
        desc->getter = JS_NewCFunction2(ctx, reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(getter)), propName.c_str(), 0, JS_CFUNC_getter, 0);
    } else {
        desc->getter = JS_UNDEFINED;
    }

    if(setter) {
        desc->setter = JS_NewCFunction2(ctx, reinterpret_cast<JSCFunction*>(reinterpret_cast<void*>(setter)), propName.c_str(), 1, JS_CFUNC_setter, 0);
    } else {
        desc->setter = JS_UNDEFINED;
    }

    return 1;
}


}; // namespace gridui_jac
