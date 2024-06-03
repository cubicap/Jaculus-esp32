#include <memory>
#include <esp_log.h>

#include <gridui.h>

#include "./builder/arm.h"
#include "./builder/bar.h"
#include "./builder/button.h"
#include "./builder/camera.h"
#include "./builder/checkbox.h"
#include "./builder/circle.h"
#include "./builder/input.h"
#include "./builder/joystick.h"
#include "./builder/led.h"
#include "./builder/orientation.h"
#include "./builder/select.h"
#include "./builder/slider.h"
#include "./builder/spinedit.h"
#include "./builder/switcher.h"
#include "./builder/text.h"

#include "gridUiFeature.h"

struct GridUiBuilderProtoBuilder : public jac::ProtoBuilder::Opaque<gridui::_GridUi>, public jac::ProtoBuilder::Properties {
    static void destroyOpaque(JSRuntime* rt, gridui::_GridUi* ptr) noexcept { }

    template<typename BuilderClassProto, typename BuilderWidgetT>
    static jac::Object builder(jac::ContextRef ctx, jac::ValueWeak thisValue, std::vector<jac::ValueWeak> args) {
        if (args.size() < 4) {
            throw jac::Exception::create(jac::Exception::Type::TypeError, "expected at least 4 arguments");
        }
        
        uint16_t uuid = 0;
        uint16_t tab = 0;
        if(args.size() >= 5) {
            uuid = args[4].to<uint16_t>();
        }

        if(args.size() >= 6) {
            tab = args[5].to<uint16_t>();
        }

        auto widget = gridui::UI.newWidget<BuilderWidgetT>(args[0].to<float>(), args[1].to<float>(), args[2].to<float>(), args[3].to<float>(), uuid, tab);
        return jac::Class<BuilderClassProto>::createInstance(ctx, widget);
    }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        using namespace gridui::builder;
        jac::FunctionFactory ff(ctx);

        proto.defineProperty("arm", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderArm, Arm>)), jac::PropFlags::Enumerable);
        proto.defineProperty("bar", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderBar, Bar>)), jac::PropFlags::Enumerable);
        proto.defineProperty("button", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderButton, Button>)), jac::PropFlags::Enumerable);
        proto.defineProperty("camera", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderCamera, Camera>)), jac::PropFlags::Enumerable);
        proto.defineProperty("checkbox", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderCheckbox, Checkbox>)), jac::PropFlags::Enumerable);
        proto.defineProperty("circle", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderCircle, Circle>)), jac::PropFlags::Enumerable);
        proto.defineProperty("input", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderInput, Input>)), jac::PropFlags::Enumerable);
        proto.defineProperty("joystick", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderJoystick, Joystick>)), jac::PropFlags::Enumerable);
        proto.defineProperty("led", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderLed, Led>)), jac::PropFlags::Enumerable);
        proto.defineProperty("orientation", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderOrientation, Orientation>)), jac::PropFlags::Enumerable);
        proto.defineProperty("select", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderSelect, Select>)), jac::PropFlags::Enumerable);
        proto.defineProperty("slider", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderSlider, Slider>)), jac::PropFlags::Enumerable);
        proto.defineProperty("spinEdit", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderSpinEdit, SpinEdit>)), jac::PropFlags::Enumerable);
        proto.defineProperty("switcher", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderSwitcher, Switcher>)), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThisVariadic(std::function(&builder<GridUiBuilderText, Text>)), jac::PropFlags::Enumerable);
    }
};

class GridUiClassInitializer {
public:
    GridUiClassInitializer() {
        jac::Class<GridUiBuilderProtoBuilder>::init("guiBuilder");

        jac::Class<GridUiBuilderArm>::init("guiArmBuilder");
        jac::Class<GridUiWidgetArm>::init("guiArm");
        jac::Class<GridUiBuilderBar>::init("guiBarBuilder");
        jac::Class<GridUiWidgetBar>::init("guiBar");
        jac::Class<GridUiBuilderButton>::init("guiButtonBuilder");
        jac::Class<GridUiWidgetButton>::init("guiButton");
        jac::Class<GridUiBuilderCamera>::init("guiCameraBuilder");
        jac::Class<GridUiWidgetCamera>::init("guiCamera");
        jac::Class<GridUiBuilderCheckbox>::init("guiCheckboxBuilder");
        jac::Class<GridUiWidgetCheckbox>::init("guiCheckbox");
        jac::Class<GridUiBuilderCircle>::init("guiCircleBuilder");
        jac::Class<GridUiWidgetCircle>::init("guiCircle");
        jac::Class<GridUiBuilderInput>::init("guiInputBuilder");
        jac::Class<GridUiWidgetInput>::init("guiInput");
        jac::Class<GridUiBuilderJoystick>::init("guiJoystickBuilder");
        jac::Class<GridUiWidgetJoystick>::init("guiJoystick");
        jac::Class<GridUiBuilderLed>::init("guiLedBuilder");
        jac::Class<GridUiWidgetLed>::init("guiLed");
        jac::Class<GridUiBuilderOrientation>::init("guiOrientationBuilder");
        jac::Class<GridUiWidgetOrientation>::init("guiOrientation");
        jac::Class<GridUiBuilderSelect>::init("guiSelectBuilder");
        jac::Class<GridUiWidgetSelect>::init("guiSelect");
        jac::Class<GridUiBuilderSlider>::init("guiSliderBuilder");
        jac::Class<GridUiWidgetSlider>::init("guiSlider");
        jac::Class<GridUiBuilderSpinEdit>::init("guiSpinEditBuilder");
        jac::Class<GridUiWidgetSpinEdit>::init("guiSpinEdit");
        jac::Class<GridUiBuilderSwitcher>::init("guiSwitcherBuilder");
        jac::Class<GridUiWidgetSwitcher>::init("guiSwitcher");
        jac::Class<GridUiBuilderText>::init("guiTextBuilder");
        jac::Class<GridUiWidgetText>::init("guiText");
    }
};

void GridUiHolder::defaultOnPacketReceived(const std::string& cmd, rbjson::Object* pkt) {
    // Let GridUI handle its packets
    if (gridui::UI.handleRbPacket(cmd, pkt))
        return;
    // ignore the rest
}


void GridUiHolder::begin(jac::ContextRef context, std::string ownerName, std::string deviceName, jac::Function builderCallback) {
    using namespace gridui;
    if(_web_server_task) {
        throw jac::Exception::create(jac::Exception::Type::Error, "gridui.begin was already called");
    }

    static GridUiClassInitializer classInit;

    // Start serving the web page
    _web_server_task = rb_web_start_no_spiffs(80, "/data");
    if(_web_server_task == NULL) {
        throw jac::Exception::create(jac::Exception::Type::InternalError, "failed to call rb_web_start_no_spiffs");
    }

    rb_web_set_not_found_callback(gridui::webserverNotFoundCallback);

    _protocol.reset(new rb::Protocol("FrantaFlinta", "Jaculus-test", "Compiled at " __DATE__ " " __TIME__, defaultOnPacketReceived));
    _protocol->start();

    UI.begin(_protocol.get());
    builderCallback.call<void>(jac::Class<GridUiBuilderProtoBuilder>::createInstance(context, &UI));
    UI.commit();
}

void GridUiHolder::end() {
    if(!_web_server_task) {
        return;
    }

    gridui::UI.end();

    rb_web_stop(_web_server_task);
    _web_server_task = nullptr;

    if(_protocol) {
        _protocol->stop();
    }
}
