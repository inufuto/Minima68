set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico CACHE STRING "Board type")

# Add executable. Default name is the project name, version 0.1

add_executable(${MAIN_PROJECT_NAME} 
        Main.cpp MasterClock.cpp Minima68Pico.cpp
        Color.cpp Ntsc.cpp
        test.cpp
)

pico_set_program_name(${MAIN_PROJECT_NAME} "${MAIN_PROJECT_NAME}")
pico_set_program_version(${MAIN_PROJECT_NAME} "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${MAIN_PROJECT_NAME} 0)
pico_enable_stdio_usb(${MAIN_PROJECT_NAME} 0)

# Add the standard library to the build
target_link_libraries(${MAIN_PROJECT_NAME}
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(${MAIN_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(${MAIN_PROJECT_NAME} 
        core
        hardware_dma
        hardware_pwm
        hardware_clocks
        hardware_irq
        )

pico_add_extra_outputs(${MAIN_PROJECT_NAME})

