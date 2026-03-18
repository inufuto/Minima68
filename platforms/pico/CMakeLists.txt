set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(PICO_BOARD pico CACHE STRING "Board type")

FetchContent_Declare(
        petitfatfs
        URL https://elm-chan.org/fsw/ff/arc/pff3a.zip
        URL_HASH SHA256=34d63bb9694f9d5c9cd21d0e8117c4222be102176aecb9f6933dfee6d6be6893
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(petitfatfs)

set(PETITFATFS_WORK_DIR ${CMAKE_CURRENT_BINARY_DIR}/petitfatfs)
file(MAKE_DIRECTORY ${PETITFATFS_WORK_DIR})
configure_file(${petitfatfs_SOURCE_DIR}/source/pff.c ${PETITFATFS_WORK_DIR}/pff.c COPYONLY)
configure_file(${petitfatfs_SOURCE_DIR}/source/pff.h ${PETITFATFS_WORK_DIR}/pff.h COPYONLY)
configure_file(${petitfatfs_SOURCE_DIR}/source/diskio.h ${PETITFATFS_WORK_DIR}/diskio.h COPYONLY)
file(WRITE ${PETITFATFS_WORK_DIR}/pffconf.h
"/* Auto-generated for Minima68 Pico build. */\n"
"#ifndef PFCONF_DEF\n"
"#define PFCONF_DEF 8088\n"
"#define PF_USE_READ 1\n"
"#define PF_USE_DIR 1\n"
"#define PF_USE_LSEEK 1\n"
"#define PF_USE_WRITE 0\n"
"#define PF_FS_FAT12 0\n"
"#define PF_FS_FAT16 1\n"
"#define PF_FS_FAT32 1\n"
"#define PF_USE_LCC 0\n"
"#define PF_CODE_PAGE 437\n"
"#endif\n"
)

add_library(petitfatfs STATIC
        ${PETITFATFS_WORK_DIR}/pff.c
        SdCardPetitFatFs.cpp
)

target_include_directories(petitfatfs PUBLIC
        ${PETITFATFS_WORK_DIR}
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(petitfatfs PUBLIC
        pico_stdlib
        hardware_spi
)

# Add executable. Default name is the project name, version 0.1

add_executable(${MAIN_PROJECT_NAME} 
        Main.cpp PrimaryClock.cpp Minima68Pico.cpp
        Color.cpp Ntsc.cpp
        SoundChannel.cpp
        Input.cpp
        Launcher.cpp
)

pico_set_program_name(${MAIN_PROJECT_NAME} "${MAIN_PROJECT_NAME}")
pico_set_program_version(${MAIN_PROJECT_NAME} "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${MAIN_PROJECT_NAME} 0)
pico_enable_stdio_usb(${MAIN_PROJECT_NAME} 0)

# Add the standard library to the build
target_link_libraries(${MAIN_PROJECT_NAME}
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(${MAIN_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    ${PICO_SDK_PATH}/lib/tinyusb/src
    ${PICO_SDK_PATH}/lib/tinyusb/src/class/hid
    ${PICO_SDK_PATH}/lib/tinyusb/src/portable/raspberrypi/rp2040
)

# Add any user requested libraries
target_link_libraries(${MAIN_PROJECT_NAME} 
        core
        petitfatfs
        hardware_dma
        hardware_pwm
        hardware_clocks
        hardware_irq
        pico_multicore
    tinyusb_host
    tinyusb_board
)

pico_add_extra_outputs(${MAIN_PROJECT_NAME})

execute_process(
    COMMAND git describe --tags --dirty --always
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
        @ONLY
)
