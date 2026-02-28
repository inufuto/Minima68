# core library
set(CORE_SOURCES
	ClockSource.cpp
)

set(CORE_HEADERS
	ClockSource.h
)

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS}
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
