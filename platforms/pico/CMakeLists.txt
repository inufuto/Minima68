set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico CACHE STRING "Board type")

# Add executable. Default name is the project name, version 0.1

add_executable(minima80i pico.cpp )

pico_set_program_name(minima80i "minima80i")
pico_set_program_version(minima80i "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(minima80i 0)
pico_enable_stdio_usb(minima80i 0)

# Add the standard library to the build
target_link_libraries(minima80i
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(minima80i PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(minima80i 
        core
        hardware_dma
        )

pico_add_extra_outputs(minima80i)

