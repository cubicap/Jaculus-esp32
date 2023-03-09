#include <jac/machine/machine.h>
#include <jac/features/asyncEventLoopFeature.h>
#include <jac/features/asyncTimersFeature.h>
#include <jac/features/yieldFeature.h>
#include <jac/features/moduleLoaderFeature.h>
#include <jac/features/filesystemFeature.h>
#include <jac/machine/values.h>

#include "features/wdtResetFeature.h"
#include "features/neopixelFeature.h"
#include "features/gpioFeature.h"
#include "features/linkIoFeature.h"

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


static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

using Machine =
    EventLoopTerminal<
    AsyncTimersFeature<
    YieldFeature<
    WdtResetFeature<
    AsyncEventLoopFeature<
    NeopixelFeature<
    GpioFeature<
    LinkIoFeature<
    ModuleLoaderFeature<
    FilesystemFeature<
    jac::MachineBase
>>>>>>>>>>;

Controller<Machine> controller([]() {
    std::stringstream oss;
    oss << esp_get_free_heap_size() << "/" << esp_get_minimum_free_heap_size();
    return oss.str();
}, []() {
    // std::stringstream oss;
    // auto stats = std::filesystem::space("/data");
    // oss << "Storage usage: \n  " << stats.available << "/" << stats.capacity << "\n";
    // return oss.str();
    return "not implemented";
});

using Mux_t = Mux<CobsPacketizer, CobsSerializer, SerialStream>;
std::unique_ptr<Mux_t> mux;

int main() {
    // Initialize NVS
    esp_vfs_fat_mount_config_t conf = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
        .disk_status_check_enable = false
    };
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
        auto machineOutput = std::make_unique<TransparentOutputStreamCommunicator>(controller.router(), 2, std::vector<int>{});
        machine.linkIo._output = std::move(machineOutput);
    });

    if (std::filesystem::exists("/data/main.js")) {
        controller.startMachine("/data/main.js");
    }
}


extern "C" void app_main() {
    main();

    vTaskDelete(NULL);
}
