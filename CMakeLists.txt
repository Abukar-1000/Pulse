cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pico_blink VERSION 0.1.0 LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(
    pico_blink 
    main.cpp
    include/Pins/Pin.h
    src/Pins/Pin.cpp
    include/Config/Secrets.h
    include/Config/Config.h
    src/Config/Config.cpp
    include/Animations/ConnectedToWifi.h
    include/Pins/PinContext.h
    include/Pins/PinContextSingleton.h
    src/Pins/PinContext.cpp
    src/Pins/PinContextSingleton.cpp
    src/Animations/ConnectedToWifi.cpp
    include/Animations/IAnimation.h
)

target_link_libraries(pico_blink pico_stdlib hardware_pwm pico_cyw43_arch_none)
pico_add_extra_outputs(pico_blink)