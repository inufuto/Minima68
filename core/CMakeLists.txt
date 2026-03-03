# core library
set(CORE_SOURCES
	ClockSource.cpp

	Cpu8085.cpp "Minima85.cpp" "AbtractEmulator.cpp")

set(CORE_HEADERS
	ClockSource.h
   "MemorySpace.h" "Uncopyable.h" "Cpu8085.h" Minima85.h "AbtractEmulator.h")

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS}
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
