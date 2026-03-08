# Audio library
set(AUDIO_SOURCES
WindowsAudioClient.cpp
)

set(AUDIO_HEADERS
WindowsAudioClient.h
)

add_library(Audio STATIC ${AUDIO_SOURCES} ${AUDIO_HEADERS})

# Include directory for other projects to use
target_include_directories(Audio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link audio libraries
target_link_libraries(Audio PRIVATE
)
