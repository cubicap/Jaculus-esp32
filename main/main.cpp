#include <jac/machine/machine.h>
#include <jac/features/eventLoopFeature.h>
#include <jac/features/timersFeature.h>
#include <jac/features/yieldFeature.h>
#include <jac/features/moduleLoaderFeature.h>
#include <jac/features/filesystemFeature.h>
#include <jac/features/basicStreamFeature.h>
#include <jac/features/stdioFeature.h>
#include <jac/machine/values.h>

#include "features/wdtResetFeature.h"
#include "features/neopixelFeature.h"
#include "features/gpioFeature.h"
#include "features/linkIoFeature.h"
#include "features/freeRTOSEventQueue.h"

#include <jac/link/mux.h>
#include <jac/link/mux.h>
#include <jac/link/encoders/cobs.h>
#include "serialStream.h"

#include <string>
#include <filesystem>
#include <sstream>

#include "esp_vfs_fat.h"
#include "freertos/task.h"


#include "util/controller.h"
#include "util/uploader.h"
#include "util/logger.h"


#if defined(CONFIG_IDF_TARGET_ESP32)
    #include "platform/esp32.h"
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
    #include "platform/esp32s3.h"
#endif


using Machine =
    EventLoopTerminal<
    TimersFeature<
    YieldFeature<
    WdtResetFeature<
    NeopixelFeature<
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

Controller<Machine> controller([]() { // get memory stats
    std::stringstream oss;
    oss << esp_get_free_heap_size() << "/" << esp_get_minimum_free_heap_size();
    return oss.str();
}, []() { // get storage stats
    // std::stringstream oss;
    // auto stats = std::filesystem::space("/data");
    // oss << "Storage usage: \n  " << stats.available << "/" << stats.capacity << "\n";
    // return oss.str();
    return "not implemented";
});

using Mux_t = Mux<CobsPacketizer, CobsSerializer, SerialStream>;
std::unique_ptr<Mux_t> mux;

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
            mux->receive();
        }
        catch (std::exception &e) {
            Logger::log(std::string("Exception: ") + e.what());
        }
        catch (...) {
            Logger::log("Unknown exception");
        }
    });

    mux = std::make_unique<Mux_t>(std::move(serialStream));
    mux->setErrorHandler([](Mux<CobsPacketizer, CobsSerializer, SerialStream>::Error error, std::vector<int> ctx) {
        std::string message = "Mux error: " + std::to_string(static_cast<int>(error)) + ", ctx: [";
        for (auto c : ctx) {
            message += std::to_string(c) + ", ";
        }
        message += "]";
        Logger::log(message);
    });
    auto handle = controller.router().subscribeTx(1, *mux);
    mux->bindRx(std::make_unique<decltype(handle)>(std::move(handle)));


    controller.onConfigureMachine([&](Machine &machine) {
        machine.stdio.out = std::make_unique<Machine::LinkWritable>(controller.machineIO().out.get());
        machine.stdio.err = std::make_unique<Machine::LinkWritable>(controller.machineIO().err.get());
        machine.stdio.in = std::make_unique<Machine::LinkReadable>(&machine, controller.machineIO().in.get());
    });

    if (std::filesystem::exists("/data/index.js")) {
        controller.startMachine("/data/index.js");
    }
}


extern "C" void app_main() {
    main();

    vTaskDelete(NULL);
}
