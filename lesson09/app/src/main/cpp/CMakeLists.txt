cmake_minimum_required(VERSION 3.4.1)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories("../../../../../common_includes/glm_0946")
include_directories("../../../../../common_includes")

add_library(
        NdkGlue SHARED
        NdkGlue.cpp
        Star.cpp
        GLES2Lesson.cpp
)

# add lib dependencies
target_link_libraries(
        NdkGlue
        android
        jnigraphics
        log
        EGL
        GLESv2
)

