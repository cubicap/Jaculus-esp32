#pragma once

#include <memory>
#include <esp_log.h>

#include <jac/machine/machine.h>
#include <jac/machine/functionFactory.h>

#include <gridui.h>

template<class Next>
class GridUiFeature : public Next {
    TaskHandle_t _web_server_task;
    std::unique_ptr<rb::Protocol> _protocol;

    static void defaultOnPacketReceived(const std::string& cmd, rbjson::Object* pkt) {
        // Let GridUI handle its packets
        if (gridui::UI.handleRbPacket(cmd, pkt))
            return;
        // ignore the rest
    }

public:
    GridUiFeature() : _web_server_task(nullptr) {

    }
    

    void initialize() {
        Next::initialize();

        using namespace gridui;

        // Start serving the web page
        _web_server_task = rb_web_start_no_spiffs(80, "/data");
        if(_web_server_task == NULL) {
            ESP_LOGE("GridUI", "failed to call rb_web_start");
            return;
        }

        rb_web_set_not_found_callback(gridui::webserverNotFoundCallback);

        _protocol.reset(new rb::Protocol("FrantaFlinta", "Jaculus-test", "Compiled at " __DATE__ " " __TIME__, defaultOnPacketReceived));
        _protocol->start();

        UI.begin(_protocol.get());

        auto gLedRed = UI.led(1, 1, 1, 1)
                  .color("red")
                  .on(true)
                  .finish();

        // The return values from finish() represent the constructed Widget in the UI.
        // They can be freely copied by value.
        auto boxBlack = UI.checkbox(4.5, 2.5, 4, 1)
                            .color("black")
                            .checked(true)
                            .text("ChkBox")
                            .finish();

        auto ledBlue = UI.led(3, 1, 1, 1)
                        .color("blue")
                        .finish();

        auto boxGreen = UI.checkbox(9.5, 2, 1.5, 3.5)
                            .color("green")
                            .fontSize(17.5)
                            .text("TestBox")
                            .onChanged([](Checkbox& b) {
                                printf("Checkbox changed: %d\n", (int)b.checked());
                            })
                            .finish();

        UI.joystick(5.5, 11.5, 5.5, 5)
            .color("red")
            .keys("wasd ")
            .text("Fire!")
            .onPositionChanged([](Joystick& joy) {
                const int x = joy.x();
                const int y = joy.x();
                if (x != 0 || y != 0) {
                    printf("Joystick value: %d %d\n", x, y);
                }
            })
            .onClick([](Joystick&) {
                printf("Fire!\n");
            })
            .finish();

        UI.button(0, 11.5, 4.5, 1.5)
            .text("BUTTON")
            .css("border", "3px solid black")
            .css("text-transform", "uppercase")
            .onPress([](Button&) {
                printf("Button pressed!\n");
            })
            .finish();

        // Commit the layout. Beyond this point, calling any builder methods on the UI is invalid.
        UI.commit();

        // Manipulating the created widgets:
        ledBlue.setColor("cyan");

        boxGreen.setText("Green!");

        boxBlack.setFontSize(20);
    }

    ~GridUiFeature() {
        gridui::UI.end();

        if(_web_server_task) {
            rb_web_stop(_web_server_task);
            _web_server_task = nullptr;
        }

        if(_protocol) {
            _protocol->stop();
        }
    }
};
