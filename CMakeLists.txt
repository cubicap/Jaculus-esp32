cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(jaculus)

fatfs_create_spiflash_image("storage" "./data" FLASH_IN_PROJECT)
