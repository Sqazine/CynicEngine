

set(NAME RenderTest)

add_executable(${NAME} TestApp.h TestApp.cpp MeshDrawPass.h MeshDrawPass.cpp )
target_include_directories(${NAME} PRIVATE ${CYNIC_INC_DIRS})
target_link_libraries(${NAME} PUBLIC ${THIRDPARTY_LIB} ${CYNIC_ENGINE_LIB_NAME})
target_compile_definitions(${NAME} PUBLIC ${COMPILE_DEFINITIONS})

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(TARGET ${NAME} PROPERTY FOLDER Test)
endif()