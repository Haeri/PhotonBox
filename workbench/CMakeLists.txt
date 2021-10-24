cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME Workbench)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
        src/*.h
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	PhotonBox)