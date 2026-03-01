# core library
set(CORE_SOURCES
	ClockSource.cpp
	
	I8085.cpp)

set(CORE_HEADERS
	ClockSource.h
   "MemorySpace.h" "Uncopyable.h" "I8085.h")

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS}
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
