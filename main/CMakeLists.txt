set(JAC_ESP32_VERSION "0.0.5")

idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS ""
    REQUIRES jac-device jac-machine jac-link
             driver pthread spiffs vfs fatfs
             SmartLeds
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE JAC_ESP32_VERSION="${JAC_ESP32_VERSION}")
