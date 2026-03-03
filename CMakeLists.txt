cmake_minimum_required(VERSION 3.16)

# オプションでターゲットを切り替え
option(BUILD_FOR_PICO "Build for Raspberry Pi Pico" ON)

if(BUILD_FOR_PICO)
    # == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
    if(WIN32)
        set(USERHOME $ENV{USERPROFILE})
    else()
        set(USERHOME $ENV{HOME})
    endif()
    set(sdkVersion 2.2.0)
    set(toolchainVersion 14_2_Rel1)
    set(picotoolVersion 2.2.0-a4)
    # ====================================================================================

    include(platforms/pico/pico_sdk_import.cmake)
    project(Minima85 C CXX ASM)
    set(MAIN_PROJECT_NAME ${PROJECT_NAME})
    pico_sdk_init()
else()
    project(Minima85 C CXX)
    set(MAIN_PROJECT_NAME ${PROJECT_NAME})
endif()

# 共通ロジック追加
add_subdirectory(core)

# ターゲットごとのビルド
if(BUILD_FOR_PICO)
    add_subdirectory(platforms/pico)
else()
    add_subdirectory(platforms/windows)
endif()
