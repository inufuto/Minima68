# Enable RC language for resource files
if(WIN32)
    enable_language(RC)
    set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo")
endif()

# Get version from Git
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()

# Set default version if Git is not available
if(NOT GIT_VERSION)
    set(GIT_VERSION "v0.0.0-dev")
endif()

# Configure version header
set(APP_VERSION "${GIT_VERSION}")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    @ONLY
)

# Add WinApi subdirectory
add_subdirectory(WinApi)

# Add Direct2D subdirectory
add_subdirectory(Direct2D)

# Add Emulator subdirectory
add_subdirectory(Emulator)

# Add Audio subdirectory
add_subdirectory(Audio)

# Source files
set(SOURCES
    WinMain.cpp
    MainWindow.cpp
    ScreenWindow.cpp
    AboutDialog.cpp
    resource.rc
    Emulator/emulator.rc
 )

# Header files
set(HEADERS
    resource.h
    MainWindow.h
    ScreenWindow.h
    AboutDialog.h
)

# Create executable
add_executable(${MAIN_PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Set resource compiler include directories
if(WIN32)
    target_include_directories(${MAIN_PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Emulator
        ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_compile_definitions(${MAIN_PROJECT_NAME} PRIVATE VERSION_INFO_AVAILABLE)
endif()

# Windows linker settings
if(WIN32)
    target_link_libraries(${MAIN_PROJECT_NAME} PRIVATE
        core
        WinApi
        Direct2D      
        Emulator
        Audio
        user32
        gdi32
        shell32
        comctl32
    )
endif()

# Set as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${MAIN_PROJECT_NAME})
