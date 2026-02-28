set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico CACHE STRING "Board type")

# Add executable. Default name is the project name, version 0.1

add_executable(trial pico.cpp )

pico_set_program_name(trial "trial")
pico_set_program_version(trial "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(trial 0)
pico_enable_stdio_usb(trial 0)

# Add the standard library to the build
target_link_libraries(trial
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(trial PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(trial 
        hardware_dma
        )

pico_add_extra_outputs(trial)

