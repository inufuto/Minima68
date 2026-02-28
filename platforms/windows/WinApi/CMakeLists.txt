# WinApi library
set(WINAPI_SOURCES
    Window.cpp
    ApiException.cpp
 ApplicationWindow.cpp "Registry.cpp" "DeviceContext.cpp" )

set(WINAPI_HEADERS
    Window.h
    ApiException.h
 ApplicationWindow.h "Registry.h" "Uncopyable.h" "ComPtr.h" "DeviceContext.h" )

add_library(WinApi STATIC ${WINAPI_SOURCES} ${WINAPI_HEADERS})

# Include directory for other projects to use
target_include_directories(WinApi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
