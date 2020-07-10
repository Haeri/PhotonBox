cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME glad)
project(${PROJECT_NAME} VERSION 2.0.0 LANGUAGES C)

file(GLOB_RECURSE SOURCE_FILES
        include/${PROJECT_NAME}/*.h
        include/KHR/*.h
        src/*.c)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)