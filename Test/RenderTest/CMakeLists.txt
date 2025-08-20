

set(NAME RenderTest)

add_executable(${NAME} TestApp.h TestApp.cpp MeshDrawPass.h MeshDrawPass.cpp )
target_include_directories(${NAME} PRIVATE ${CYNIC_INC_DIRS})
target_link_libraries(${NAME} PUBLIC ${THIRDPARTY_LIB} ${CYNIC_ENGINE_LIB_NAME})
target_compile_definitions(${NAME} PUBLIC ${COMPILE_DEFINITIONS})
target_compile_definitions(${NAME} PRIVATE SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
target_compile_definitions(${NAME} PRIVATE ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(TARGET ${NAME} PROPERTY FOLDER Test)
endif()