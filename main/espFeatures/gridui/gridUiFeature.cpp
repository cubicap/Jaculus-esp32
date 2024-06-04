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

class GridUiBuilderProtoBuilder : public jac::ProtoBuilder::Opaque<GridUiHolder>, public jac::ProtoBuilder::Properties {
public:
    static void destroyOpaque(JSRuntime* rt, GridUiHolder* ptr) noexcept { }

    template<typename BuilderWidgetT, jac::Object(*ProtoBuilder)(jac::ContextRef, std::map<intptr_t, jac::Object>&)>
    static jac::Object builder(jac::ContextRef ctx, jac::ValueWeak thisVal, std::vector<jac::ValueWeak> args) {
        auto& holder = *GridUiBuilderProtoBuilder::getOpaque(ctx, thisVal);

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

        auto obj = jac::Object::create(ctx);
        JS_SetOpaque(obj.getVal(), widget);

        auto protoKey = (intptr_t)widget->widgetTypeName();
        auto itr = holder._protos.find(protoKey);
        if(itr == holder._protos.end()) {
            auto proto = ProtoBuilder(ctx, holder._protos);
            obj.setPrototype(proto);
            holder._protos.emplace(protoKey, proto);
        } else {
            obj.setPrototype(itr->second);
        }
        return obj;
    }

    static void addProperties(JSContext* ctx, jac::Object proto) {
        using namespace gridui::builder;
        jac::FunctionFactory ff(ctx);

        proto.defineProperty("arm", ff.newFunctionThisVariadic(std::function(&builder<Arm, griduiBuilderArmProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("bar", ff.newFunctionThisVariadic(std::function(&builder<Bar, griduiBuilderBarProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("button", ff.newFunctionThisVariadic(std::function(&builder<Button, griduiBuilderButtonProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("camera", ff.newFunctionThisVariadic(std::function(&builder<Camera, griduiBuilderCameraProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("checkbox", ff.newFunctionThisVariadic(std::function(&builder<Checkbox, griduiBuilderCheckboxProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("circle", ff.newFunctionThisVariadic(std::function(&builder<Circle, griduiBuilderCircleProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("input", ff.newFunctionThisVariadic(std::function(&builder<Input, griduiBuilderInputProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("joystick", ff.newFunctionThisVariadic(std::function(&builder<Joystick, griduiBuilderJoystickProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("led", ff.newFunctionThisVariadic(std::function(&builder<Led, griduiBuilderLedProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("orientation", ff.newFunctionThisVariadic(std::function(&builder<Orientation, griduiBuilderOrientationProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("select", ff.newFunctionThisVariadic(std::function(&builder<Select, griduiBuilderSelectProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("slider", ff.newFunctionThisVariadic(std::function(&builder<Slider, griduiBuilderSliderProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("spinEdit", ff.newFunctionThisVariadic(std::function(&builder<SpinEdit, griduiBuilderSpinEditProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("switcher", ff.newFunctionThisVariadic(std::function(&builder<Switcher, griduiBuilderSwitcherProto>)), jac::PropFlags::Enumerable);
        proto.defineProperty("text", ff.newFunctionThisVariadic(std::function(&builder<Text, griduiBuilderTextProto>)), jac::PropFlags::Enumerable);


    }
};

class GridUiClassInitializer {
public:
    GridUiClassInitializer() {
        jac::Class<GridUiBuilderProtoBuilder>::init("guiBuilder");
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
    builderCallback.call<void>(jac::Class<GridUiBuilderProtoBuilder>::createInstance(context, this));
    _protos.clear();
    UI.commit();
}

void GridUiHolder::end() {
    if(!_web_server_task) {
        return;
    }

    _protos.clear();

    gridui::UI.end();

    rb_web_stop(_web_server_task);
    _web_server_task = nullptr;

    if(_protocol) {
        _protocol->stop();
    }
}
