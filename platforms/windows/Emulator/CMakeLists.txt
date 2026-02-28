# Emulator library
set(EMULATOR_SOURCES
MasterClock.cpp
)

set(EMULATOR_HEADERS
MasterClock.h
)

add_library(Emulator STATIC ${EMULATOR_SOURCES} ${EMULATOR_HEADERS})

# Include directory for other projects to use
target_include_directories(Emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
