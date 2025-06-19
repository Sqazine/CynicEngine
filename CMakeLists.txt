cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME CynicEngine  VERSION 0.1.0 LANGUAGES C CXX)

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

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/3rd")
set(SDL_INC_DIR "${THIRD_PARTY_DIR}/SDL/include")

find_package(Vulkan REQUIRED)
add_subdirectory(${THIRD_PARTY_DIR}/SDL)

if(WIN32)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(TARGET SDL_uclibc PROPERTY FOLDER 3rd/SDL3)
    set_property(TARGET SDL3-shared PROPERTY FOLDER 3rd/SDL3)
    set_property(TARGET SDL3_test PROPERTY FOLDER 3rd/SDL3)
endif()

file(GLOB GENERATED_SRC "${GENERATED_DIR}/*.cpp" "${GENERATED_DIR}/*.h")
file(GLOB CONFIG_SRC "${CMAKE_SOURCE_DIR}/Config/*.cpp" "${CMAKE_SOURCE_DIR}/Config/*.h")
file(GLOB CORE_SRC "${CMAKE_SOURCE_DIR}/Core/*.cpp" "${CMAKE_SOURCE_DIR}/Core/*.h")
file(GLOB EDITOR_SRC "${CMAKE_SOURCE_DIR}/Editor/*.cpp" "${CMAKE_SOURCE_DIR}/Editor/*.h")
file(GLOB GFX_SRC "${CMAKE_SOURCE_DIR}/Gfx/*.cpp" "${CMAKE_SOURCE_DIR}/Gfx/*.h")
file(GLOB GFX_VULKAN_SRC "${CMAKE_SOURCE_DIR}/Gfx/VK/*.cpp" "${CMAKE_SOURCE_DIR}/Gfx/VK/*.h")
file(GLOB MATH_SRC "${CMAKE_SOURCE_DIR}/Math/*.cpp" "${CMAKE_SOURCE_DIR}/Math/*.h")
file(GLOB PLATFORM_SRC "${CMAKE_SOURCE_DIR}/Platform/*.cpp" "${CMAKE_SOURCE_DIR}/Platform/*.h")
file(GLOB PLATFORM_SDL3_SRC "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.cpp" "${CMAKE_SOURCE_DIR}/Platform/SDL3/*.h")
file(GLOB RENDER_SRC "${CMAKE_SOURCE_DIR}/Render/*.cpp" "${CMAKE_SOURCE_DIR}/Render/*.h")

source_group("generated" FILES ${GENERATED_SRC})
source_group("Config" FILES ${CONFIG_SRC})
source_group("Core" FILES ${CORE_SRC})
source_group("Editor" FILES ${EDITOR_SRC})
source_group("Gfx" FILES ${GFX_SRC})
source_group("Gfx/VK" FILES ${GFX_VULKAN_SRC})
source_group("Math" FILES ${MATH_SRC})
source_group("Platform" FILES ${PLATFORM_SRC})
source_group("Platform/SDL3" FILES ${PLATFORM_SDL3_SRC})
source_group("Render" FILES ${RENDER_SRC})

set(CYNIC_SRC
    ${GENERATED_SRC}
    ${CONFIG_SRC}
    ${CORE_SRC}
    ${EDITOR_SRC}
    ${GFX_SRC}
    ${GFX_VULKAN_SRC}
    ${MATH_SRC}
    ${PLATFORM_SRC}
    ${PLATFORM_SDL3_SRC}
    ${RENDER_SRC}
)

add_executable(${PROJECT_NAME} ${CYNIC_SRC})

set(CYNIC_INC_DIRS ${CMAKE_SOURCE_DIR}
    ${GENERATED_DIR}
    Core
    Editor
    Gfx
    Gfx/VK
    Math
    Platform
    Platform/SDL3
    Render
    ${SDL_INC_DIR})
                    
target_include_directories(${PROJECT_NAME} PUBLIC ${CYNIC_INC_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    Vulkan::Vulkan
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${CURRENT_PLATFORM} ${VULKAN_HPP_DISPATCH_LOADER_DYNAMIC} ${VULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL})

if(CYNIC_ENGINE_UTF8_ENCODE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC CYNIC_ENGINE_UTF8_ENCODE)
endif()


if(${CURRENT_PLATFORM} STREQUAL "PLATFORM_WINDOWS")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE "/wd4251;" "/bigobj;" "/MD;")
    endif()
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()