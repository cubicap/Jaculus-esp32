idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS ""
    REQUIRES jac-device jac-machine jac-link
             driver pthread spiffs vfs fatfs
             SmartLeds
)
