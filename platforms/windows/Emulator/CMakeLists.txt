# Emulator library
set(EMULATOR_SOURCES
PrimaryClock.cpp
Minima68Win.cpp
EmulatorWindow.cpp
  "RegisterWindow.cpp"
  "ListWindow.cpp" 
  "StatusWindow.cpp" 
  "TitledPane.cpp" 
  "MemoryWindow.cpp" "AssemblyWindow.cpp" "BreakpointWindow.cpp" BreakPointDialog.cpp
 "SoundChannel.cpp")

set(EMULATOR_HEADERS
PrimaryClock.h
EmulatorWindow.h
 "Minima68Win.h" "RegisterWindow.h" "ListWindow.h" "StatusWindow.h" "TitledPane.h" "MemoryWindow.h" "AssemblyWindow.h" "BreakpointWindow.h" "SoundChannel.h")

add_library(Emulator STATIC ${EMULATOR_SOURCES} ${EMULATOR_HEADERS})

# Include directory for other projects to use
target_include_directories(Emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include generated version header
target_include_directories(Emulator PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/..)
target_compile_definitions(Emulator PRIVATE VERSION_INFO_AVAILABLE)
