#==========================================
#Project options
#==========================================
cmake_minimum_required(VERSION 2.8)

#==========================================
#Project name
#==========================================
project(3DGraphics)

#==========================================
#Building options
#==========================================
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

#==========================================
#Libraries path : glm, sfml, glew, opengl, freetype
#==========================================
set(GLM_INCLUDE_DIRS "" CACHE PATH "path to glm headers")
include_directories(${GLM_INCLUDE_DIRS})

set(SFML_INCLUDE_DIRS "" CACHE PATH "path to sfml headers")
set(SFML_SYSTEM_LIBRARIES "" CACHE FILEPATH "path to sfml library")
set(SFML_WINDOW_LIBRARIES "" CACHE FILEPATH "path to sfml library")
set(SFML_GRAPHICS_LIBRARIES "" CACHE FILEPATH "path to sfml library")
include_directories(${SFML_INCLUDE_DIRS})

set(GLEW_INCLUDE_DIRS "" CACHE PATH "path to glew headers")
set(GLEW_LIBRARIES "" CACHE FILEPATH "path to glew library")
include_directories(${GLEW_INCLUDE_DIRS})

set(FREETYPE_INCLUDE_DIRS "" CACHE PATH "path to Freetype headers")
set(FREETYPE_LIBRARIES "" CACHE FILEPATH "path to Freetype library")
include_directories(${FREETYPE_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

#==============================================
#Project sources : src, include, shader, exe
#==============================================
file(
    GLOB_RECURSE
    HEADER_FILES
    include/*.hpp
    )

file(
    GLOB_RECURSE
    SOURCE_FILES
    src/*.cpp
    )

set(
    GLOB_RECURSE
    SHADER_FILES
    shaders/*.glsl
    )

set(EXECUTABLE_NAME main)

#==============================================
#Project executable definition
#==============================================
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

#==============================================
#Linking with libraries
#==============================================
if (OPENGL_FOUND)
    target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_SYSTEM_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_WINDOW_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_GRAPHICS_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${FREETYPE_LIBRARIES})
