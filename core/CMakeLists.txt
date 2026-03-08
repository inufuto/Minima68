# core library
set(CORE_SOURCES
	ClockSource.cpp
	"Minima68.cpp" "AbtractEmulator.cpp" "Cpu6800.cpp"
)

set(CORE_HEADERS
	ClockSource.h
   "MemorySpace.h" "Uncopyable.h"  Minima68.h "AbtractEmulator.h" "Cpu6800.h"
   "Video.h"
)

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS}
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
