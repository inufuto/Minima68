cmake_minimum_required(VERSION 3.10)
project(Minima80i)

# Add WinApi subdirectory
add_subdirectory(WinApi)

# Add Direct2D subdirectory
add_subdirectory(Direct2D)

# Add Emulator subdirectory
add_subdirectory(Emulator)

# Source files
set(SOURCES
    Main/WinMain.cpp
    Main/MainWindow.cpp
    Main/resource.rc
)

# Header files
set(HEADERS
    Main/resource.h
    Main/MainWindow.h
)

# Create executable
add_executable(${MAIN_PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Set resource compiler include directories
if(WIN32)
    target_include_directories(${MAIN_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Windows linker settings
if(WIN32)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE
        core
        WinApi
        Direct2D      
        Emulator
        user32
        gdi32
        shell32
        comctl32
    )
endif()
