cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD pico_w)
include_directories(include)
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
    include/Connections/TCP/Server.h
    include/Connections/IConnection.h
    src/Connections/TCP/Server.cpp
    include/Connections/TCP/Resources/PCBDeleter.h
    include/lwipopts.h
    include/Animations/Definitions/Percise.h
    src/Animations/Definitions/Percise.cpp
    include/Animations/Parsing/Parser.h
    include/Animations/AnimationContext.h
    src/Animations/AnimationContext.cpp
    include/Locks/MutexLock.h
    src/Locks/MutexLock.cpp
    include/Locks/MutexLockSingleton.h
    src/Locks/MutexLockSingleton.cpp
    include/Animations/FailedToConnect.h
    src/Animations/FailedToConnect.cpp
    include/Animations/RequestReceived.h
    src/Animations/RequestReceived.cpp
)

target_link_libraries(
    pico_blink 
    pico_stdlib 
    pico_sync
    hardware_pwm 
    pico_cyw43_arch_lwip_threadsafe_background
    pico_multicore
    pico_cyw43_arch_none
)

pico_add_extra_outputs(pico_blink)