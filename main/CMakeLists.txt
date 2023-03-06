set(UTIL
    "util/uploader.cpp"
    "util/logger.cpp"
)

idf_component_register(
    SRCS "main.cpp" ${UTIL}
    INCLUDE_DIRS ""
    REQUIRES jac-machine jac-link
             driver pthread spiffs vfs fatfs
             SmartLeds
)
