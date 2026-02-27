cmake_minimum_required(VERSION 3.10)
project(Project)

# Add WinApi subdirectory
add_subdirectory(src/WinApi)

# Add Direct2D subdirectory
add_subdirectory(src/Direct2D)

# Source files
set(SOURCES
    src/Main/WinMain.cpp
    src/Main/MainWindow.cpp
    src/Main/resource.rc
)

# Header files
set(HEADERS
    src/Main/resource.h
    src/Main/MainWindow.h
)

# Create executable
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Set resource compiler include directories
if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

# Windows linker settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        WinApi
        Direct2D
        user32
        gdi32
        shell32
        comctl32
    )
endif()
