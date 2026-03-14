# WinApi library
set(WINAPI_SOURCES
    Window.cpp
    ApiException.cpp
 ApplicationWindow.cpp "Registry.cpp" "DeviceContext.cpp"  "Font.cpp" "Dialog.cpp" "Control.cpp" "FileDialog.cpp")

set(WINAPI_HEADERS
    Window.h
    ApiException.h
 ApplicationWindow.h "Registry.h" "ComPtr.h" "DeviceContext.h"  "Font.h" "Dialog.h" "Control.h" "FileDialog.h")

add_library(WinApi STATIC ${WINAPI_SOURCES} ${WINAPI_HEADERS})

# Include directory for other projects to use
target_include_directories(WinApi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
