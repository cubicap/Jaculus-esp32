#include <jac/machine/machine.h>
#include <jac/machine/values.h>
#include <jac/features/eventLoopFeature.h>
#include <jac/features/timersFeature.h>
#include <jac/features/moduleLoaderFeature.h>
#include <jac/features/filesystemFeature.h>
#include <jac/features/basicStreamFeature.h>
#include <jac/features/stdioFeature.h>

#include <jac/device/device.h>
#include <jac/device/logger.h>
#include <jac/features/linkIoFeature.h>

#include <jac/link/mux.h>
#include <jac/link/encoders/cobs.h>

#include "espFeatures/neopixelFeature.h"
#include "espFeatures/gpioFeature.h"
#include "espFeatures/freeRTOSEventQueue.h"
#include "espFeatures/ledcFeature.h"
#include "espFeatures/adcFeature.h"

#include "util/serialStream.h"

#include <string>
#include <filesystem>
#include <sstream>

#include "esp_vfs_fat.h"
#include "freertos/task.h"

#include "esp_pthread.h"


#if defined(CONFIG_IDF_TARGET_ESP32)
    #include "platform/esp32.h"
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    #include "platform/esp32s3.h"
#endif


using Machine =
    EventLoopTerminal<
    TimersFeature<
    NeopixelFeature<
    AdcFeature<
    LedcFeature<
    GpioFeature<
    EventLoopFeature<
    ModuleLoaderFeature<
    FilesystemFeature<
    PlatformInfoFeature<
    LinkIoFeature<
    StdioFeature<
    BasicStreamFeature<
    FreeRTOSEventQueueFeature<
    jac::MachineBase
>>>>>>>>>>>>>>;

jac::Device<Machine> device(
    "/data",
    []() { // get memory stats
        std::stringstream oss;
        oss << esp_get_free_heap_size() << "/" << esp_get_minimum_free_heap_size();
        return oss.str();
    },
    []() { // get storage stats
        // std::stringstream oss;
        // auto stats = std::filesystem::space("/data");
        // oss << "Storage usage: \n  " << stats.available << "/" << stats.capacity << "\n";
        // return oss.str();
        return "not implemented";
    }
);

using Mux_t = jac::Mux<jac::CobsPacketizer, jac::CobsSerializer>;
std::unique_ptr<Mux_t> muxSerial;

int main() {
    // Initialize vfs
    esp_vfs_fat_mount_config_t conf = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
        .disk_status_check_enable = false
    };

    static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    ESP_ERROR_CHECK(esp_vfs_fat_spiflash_mount_rw_wl("/data", "storage", &conf, &s_wl_handle));


    auto serialStream = std::make_unique<SerialStream>(UART_NUM_0, 921600, 4096, 0);
    serialStream->onData([]() noexcept {
        try {
            muxSerial->receive();
        }
        catch (std::exception &e) {
            jac::Logger::log(std::string("Exception: ") + e.what());
        }
        catch (...) {
            jac::Logger::log("Unknown exception");
        }
    });

    muxSerial = std::make_unique<Mux_t>(std::move(serialStream));
    muxSerial->setErrorHandler([](Mux_t::Error error, std::vector<int> ctx) {
        std::string message = "Mux error: " + std::to_string(static_cast<int>(error)) + ", ctx: [";
        for (auto c : ctx) {
            message += std::to_string(c) + ", ";
        }
        message += "]";
        jac::Logger::log(message);
    });
    auto handle = device.router().subscribeTx(1, *muxSerial);
    muxSerial->bindRx(std::make_unique<decltype(handle)>(std::move(handle)));


    device.onConfigureMachine([&](Machine &machine) {
        device.machineIO().in->clear();

        machine.stdio.out = std::make_unique<Machine::LinkWritable>(device.machineIO().out.get());
        machine.stdio.err = std::make_unique<Machine::LinkWritable>(device.machineIO().err.get());
        machine.stdio.in = std::make_unique<Machine::LinkReadable>(&machine, device.machineIO().in.get());

        esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
        cfg.stack_size = 2 * 1024;
        cfg.inherit_cfg = true;
        esp_pthread_set_cfg(&cfg);
    });

    esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
    cfg.stack_size = 8 * 1024;
    cfg.inherit_cfg = true;
    esp_pthread_set_cfg(&cfg);

    device.start();

    device.startMachine("index.js");
}


extern "C" void app_main() {
    main();

    vTaskDelete(NULL);
}
