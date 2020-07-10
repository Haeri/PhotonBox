cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME Sandbox)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
        include/${PROJECT_NAME}/*.h
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	PhysXPvdSDK_static_64
	PhysXExtensions_static_64
	PhysXFoundation_64
	PhysXCommon_64
	PhysX_64
	PhotonBox)
