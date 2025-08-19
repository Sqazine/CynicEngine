add_executable(FrameGraphTest FrameGraphTest.cpp)
target_include_directories(FrameGraphTest PRIVATE ${CYNIC_INC_DIRS})

if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set_property(TARGET FrameGraphTest PROPERTY FOLDER Test)
endif()