# Emulator library
set(EMULATOR_SOURCES
MasterClock.cpp
Minima68Win.cpp
EmulatorWindow.cpp
  "RegisterWindow.cpp" "ListWindow.cpp" "StatusWindow.cpp" "TitledPane.cpp" "MemoryWindow.cpp" "AssemblyWindow.cpp" "BreakpointWindow.cpp")

set(EMULATOR_HEADERS
MasterClock.h
EmulatorWindow.h
 "Minima68Win.h" "RegisterWindow.h" "ListWindow.h" "StatusWindow.h" "TitledPane.h" "MemoryWindow.h" "AssemblyWindow.h" "BreakpointWindow.h")

add_library(Emulator STATIC ${EMULATOR_SOURCES} ${EMULATOR_HEADERS})

# Include directory for other projects to use
target_include_directories(Emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
