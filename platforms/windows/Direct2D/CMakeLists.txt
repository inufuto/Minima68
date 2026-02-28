# Direct2D library
set(DIRECT2D_SOURCES
RenderTarget.cpp
 "RenderTargetWindow.cpp" "TextFormat.cpp" "Direct2DBrush.cpp" "WicBitmap.cpp" "Direct2DBitmap.cpp")

set(DIRECT2D_HEADERS
RenderTarget.h
 "RenderTargetWindow.h" "TextFormat.h" "Direct2DBrush.h" "WicBitmap.h" "Direct2DBitmap.h")

add_library(Direct2D STATIC ${DIRECT2D_SOURCES} ${DIRECT2D_HEADERS})

# Include directory for other projects to use
target_include_directories(Direct2D PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link Direct2D and related libraries
target_link_libraries(Direct2D PRIVATE
    d2d1
    dwrite
)
