cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME Sandbox)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
        include/${PROJECT_NAME}/*.h
        src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# Make res folder available inside the cmake build
set (engine_src "${CMAKE_SOURCE_DIR}/res")
set (engine_dest "${CMAKE_CURRENT_BINARY_DIR}/pb_res")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${engine_src} ${engine_dest} 
    DEPENDS ${engine_dest}
    COMMENT "symbolic link engine resources folder from ${engine_src} => ${engine_dest}"
)

set (src "${CMAKE_CURRENT_SOURCE_DIR}/res")
set (dest "${CMAKE_CURRENT_BINARY_DIR}/res")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest} 
    DEPENDS ${dest}
    COMMENT "symbolic link engine resources folder from ${src} => ${dest}"
)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	PhotonBox)