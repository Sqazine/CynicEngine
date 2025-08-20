cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME CynicEngine  VERSION 0.1.0 LANGUAGES C CXX)

if(MSVC OR ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(CYNIC_ENGINE_LIB_NAME libCynicEngine CACHE INTERNAL "CynicEngine library name")
else()
    set(CYNIC_ENGINE_LIB_NAME CynicEngine CACHE INTERNAL "CynicEngine library name")
endif()

project(${PROJECT_NAME})

configure_file(Config/Version.h.in generated/Version.h @ONLY)
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED OFF)

set(CMAKE_DEBUG_POSTFIX ${CYS_DEBUG_POSTFIX}) 
set(CMAKE_RELEASE_POSTFIX ${CYS_RELEASE_POSTFIX})

set(CYNIC_ENGINE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CYNIC_ENGINE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CYNIC_ENGINE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CYNIC_ENGINE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CYNIC_ENGINE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CYNIC_ENGINE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CYNIC_ENGINE_BINARY_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "current platform: Linux")
    set(CURRENT_PLATFORM PLATFORM_LINUX)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "current platform: Windows")
    set(CURRENT_PLATFORM PLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    message(STATUS "current platform: FreeBSD")
    set(CURRENT_PLATFORM PLATFORM_FREEBSD)
else ()
    message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
    set(CURRENT_PLATFORM PLATFORM_OTHER)
endif ()

message(STATUS "CURRENT_PLATFORM=${CURRENT_PLATFORM}")

option(CYNIC_ENGINE_UTF8_ENCODE "use utf8 encode" ON)
option(CYNIC_ENGINE_BUILD_TEST "build test example" ON)

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rd")
set(SDL_INC_DIR "${THIRD_PARTY_DIR}/SDL/include")
set(CYS_INC_DIR "${THIRD_PARTY_DIR}/CynicScript")
set(SPIRV_REFLECT_INC_DIR "${THIRD_PARTY_DIR}/SPIRV-Reflect")

set(SPIRV_REFLECT_STATIC_LIB ON)

find_package(Vulkan REQUIRED)
add_subdirectory(${THIRD_PARTY_DIR}/SDL EXCLUDE_FROM_ALL)
add_subdirectory(${THIRD_PARTY_DIR}/SPIRV-Reflect EXCLUDE_FROM_ALL)
add_subdirectory(${THIRD_PARTY_DIR}/CynicScript)

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    set_property(TARGET SDL_uclibc PROPERTY FOLDER 3rd/SDL3)
    set_property(TARGET SDL3-shared PROPERTY FOLDER 3rd/SDL3)
    set_property(TARGET SDL3_test PROPERTY FOLDER 3rd/SDL3)

    set_property(TARGET spirv-reflect-static PROPERTY FOLDER 3rd/SPIRV-Reflect)

    set_property(TARGET ${CYS_LIB_NAME} PROPERTY FOLDER 3rd/CynicScript)
    set_property(TARGET ${CYS_EXE_NAME} PROPERTY FOLDER 3rd/CynicScript)
endif()

file(GLOB GENERATED_SRC "${GENERATED_DIR}/*.cpp" "${GENERATED_DIR}/*.h"  "${GENERATED_DIR}/*.inl")
file(GLOB CONFIG_SRC "${CMAKE_SOURCE_DIR}/Config/*.cpp" "${CMAKE_SOURCE_DIR}/Config/*.h" "${CMAKE_SOURCE_DIR}/Config/*.inl")
file(GLOB CORE_SRC "${CMAKE_SOURCE_DIR}/Core/*.cpp" "${CMAKE_SOURCE_DIR}/Core/*.h" "${CMAKE_SOURCE_DIR}/Core/*.inl")
file(GLOB EDITOR_SRC "${CMAKE_SOURCE_DIR}/Editor/*.cpp" "${CMAKE_SOURCE_DIR}/Editor/*.h" "${CMAKE_SOURCE_DIR}/Editor/*.inl")
file(GLOB GFX_SRC "${CMAKE_SOURCE_DIR}/Gfx/*.cpp" "${CMAKE_SOURCE_DIR}/Gfx/*.h" "${CMAKE_SOURCE_DIR}/Gfx/*.inl")
file(GLOB GFX_VULKAN_SRC "${CMAKE_SOURCE_DIR}/Gfx/VK/*.cpp" "${CMAKE_SOURCE_DIR}/Gfx/VK/*.h" "${CMAKE_SOURCE_DIR}/Gfx/VK/*.inl")
file(GLOB LOGGER_SRC "${CMAKE_SOURCE_DIR}/Logger/*.cpp" "${CMAKE_SOURCE_DIR}/Logger/*.h" "${CMAKE_SOURCE_DIR}/Logger/*.inl")
file(GLOB MATH_SRC "${CMAKE_SOURCE_DIR}/Math/*.cpp" "${CMAKE_SOURCE_DIR}/Math/*.h" "${CMAKE_SOURCE_DIR}/Math/*.inl")
file(GLOB PLATFORM_SRC "${CMAKE_SOURCE_DIR}/Platform/*.cpp" "${CMAKE_SOURCE_DIR}/Platform/*.h" "${CMAKE_SOURCE_DIR}/Platform/*.inl")
file(GLOB PLATFORM_SDL3_SRC "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.cpp" "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.h" "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.inl")
file(GLOB RENDER_SRC "${CMAKE_SOURCE_DIR}/Render/*.cpp" "${CMAKE_SOURCE_DIR}/Render/*.h" "${CMAKE_SOURCE_DIR}/Render/*.inl")
file(GLOB FRAMEGRAPH_SRC "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.cpp" "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.h" "${CMAKE_SOURCE_DIR}/Render/FrameGraph/*.inl")

source_group("generated" FILES ${GENERATED_SRC})
source_group("Config" FILES ${CONFIG_SRC})
source_group("Core" FILES ${CORE_SRC})
source_group("Editor" FILES ${EDITOR_SRC})
source_group("Gfx" FILES ${GFX_SRC})
source_group("Gfx/VK" FILES ${GFX_VULKAN_SRC})
source_group("Logger" FILES ${LOGGER_SRC})
source_group("Math" FILES ${MATH_SRC})
source_group("Platform" FILES ${PLATFORM_SRC})
source_group("Platform/SDL3" FILES ${PLATFORM_SDL3_SRC})
source_group("Render" FILES ${RENDER_SRC})
source_group("Render/FrameGraph" FILES ${FRAMEGRAPH_SRC})

set(CYNIC_SRC
    ${GENERATED_SRC}
    ${CONFIG_SRC}
    ${CORE_SRC}
    ${GFX_SRC}
    ${GFX_VULKAN_SRC}
    ${LOGGER_SRC}
    ${MATH_SRC}
    ${PLATFORM_SRC}
    ${PLATFORM_SDL3_SRC}
    ${RENDER_SRC}
    ${FRAMEGRAPH_SRC}
)

set(CYNIC_INC_DIRS 
    ${CMAKE_SOURCE_DIR}
    ${GENERATED_DIR}
    ${CMAKE_SOURCE_DIR}/Core
    ${CMAKE_SOURCE_DIR}/Gfx
    ${CMAKE_SOURCE_DIR}/Gfx/VK
    ${CMAKE_SOURCE_DIR}/Logger
    ${CMAKE_SOURCE_DIR}/Math
    ${CMAKE_SOURCE_DIR}/Platform
    ${CMAKE_SOURCE_DIR}/Platform/SDL3
    ${CMAKE_SOURCE_DIR}/Render
    ${CMAKE_SOURCE_DIR}/Render/FrameGraph

    ${SDL_INC_DIR}
    ${CYS_INC_DIR}
    ${SPIRV_REFLECT_INC_DIR}
)

set(THIRDPARTY_LIB 
    SDL3::SDL3
    Vulkan::Vulkan
    spirv-reflect-static
    ${CYS_LIB_NAME}
)

set(COMPILE_DEFINITIONS
    ${CURRENT_PLATFORM} 
    ${VULKAN_HPP_DISPATCH_LOADER_DYNAMIC} 
    ${VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL}
)

if(CYNIC_ENGINE_UTF8_ENCODE)
    list(APPEND COMPILE_DEFINITIONS CYNIC_ENGINE_UTF8_ENCODE)
endif()

add_library(${CYNIC_ENGINE_LIB_NAME} ${CYNIC_SRC})
target_include_directories(${CYNIC_ENGINE_LIB_NAME}  PUBLIC ${CYNIC_INC_DIRS})
target_link_libraries(${CYNIC_ENGINE_LIB_NAME} PUBLIC ${THIRDPARTY_LIB})
target_compile_definitions(${CYNIC_ENGINE_LIB_NAME} PUBLIC ${COMPILE_DEFINITIONS})

if(CYNIC_ENGINE_BUILD_TEST)
    add_subdirectory(Test)
endif()

add_executable(${PROJECT_NAME} ${EDITOR_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CYNIC_INC_DIRS} ${CMAKE_SOURCE_DIR}/Editor)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CYNIC_ENGINE_LIB_NAME} ${THIRDPARTY_LIB})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPILE_DEFINITIONS})


if(${CURRENT_PLATFORM} STREQUAL "PLATFORM_WINDOWS")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE "/wd4251;" "/bigobj;")
    endif()
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()