cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME PhotonBox)
project(${PROJECT_NAME} LANGUAGES CXX)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PB_STANDALONE TRUE)
endif()

option(BUILD_SHARED_LIBS "Build as shared/dynamic library" OFF)
option(PB_BUILD_EXAMPLES "Build the example programs" ${PB_STANDALONE})
option(DYNAMIC_RUNTIME_LIBRARY "Set runtime library to dynamic" ${BUILD_SHARED_LIBS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINE_HIDDEN YES)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_FILES
        include/${PROJECT_NAME}/*.h
        src/*.h
        src/*.cpp)

# Microsoft Runtime Library shenanigens
if(NOT DYNAMIC_RUNTIME_LIBRARY) 
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if (BUILD_SHARED_LIBS)
    add_compile_definitions(PB_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    SOVERSION 0
    VERSION 0.8.1
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(workbench)



# ---------------- EXAMPLES ---------------------#

if (PB_BUILD_EXAMPLES)
    add_subdirectory(examples/Sandbox)

    set_target_properties(Sandbox PROPERTIES FOLDER examples)

    if (BUILD_SHARED_LIBS)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            DEPENDS Sandbox
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}/examples/Sandbox/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
            COMMENT "Copy library"
    )
  endif()
endif()



# Hack until vcpkg fixes physx setup

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug>/lib)

if(APPLE)    
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        PhysXPvdSDK_static_64
        PhysXExtensions_static_64
        PhysXFoundation_static_64
        PhysXCommon_static_64
        PhysX_static_64
    )
elseif(WIN32) 
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        PhysXPvdSDK_static_64
        PhysXExtensions_static_64
        PhysXFoundation_static_64
        PhysXCommon_static_64
        PhysX_static_64
        -static
    )
else()
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        -Wl,--start-group
        PhysXPvdSDK_static_64
        PhysXExtensions_static_64
        PhysXFoundation_static_64
        PhysXCommon_static_64
        PhysX_static_64
        -Wl,--end-group
    )
endif()

    

# ---------------- DEPENDENCIES ---------------------#

set(EXTERN_DIR external/)


# VCPKG INCLUDES

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# freetype
find_package(freetype CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

# imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

# zlib
find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

# physx
#find_package(physx CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PUBLIC physx)