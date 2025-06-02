cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 包含自己写的包含头文件小工具
include(${CMAKE_SOURCE_DIR}/find_header.cmake)

# Set the project name
set(CMAKE_PROJECT_NAME xbox_controller)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES
    "Drivers/*.c"
    "Core/*.c"
    "Module/*.c"
    "App/*.c"
    "Middlewares/*.c"
    
)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/Core)
include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/Module)
include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/Drivers)
include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/App)
include_sub_directories_recursively(${CMAKE_SOURCE_DIR}/Middlewares)




# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# add_custom_command 添加自定义构建步骤
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD # 指定自定义构建的对象是.elf
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex # 使用objcopy工具将elf文件格式转换成hex文件
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin # 使用ogjcopy工具将elf文件转换为二进制格式
    COMMENT "Building hex & bin file..." # 这些注释在构建过程中显示
    COMMENT "EXCUTABLE SIZE:"
    # COMMAND ${SIZE} ${PROJECT_NAME}.elf # 使用size工具显示elf文件的大小信息
    
)