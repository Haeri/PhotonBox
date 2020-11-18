cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME PhotonBox)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(${PROJECT_NAME} VERSION 0.8.0 LANGUAGES CXX)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PB_STANDALONE TRUE)
endif()

option(BUILD_SHARED_LIBS "Build as shared/dynamic library" OFF)
option(PB_BUILD_EXAMPLES "Build the example programs" ${PB_STANDALONE})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_FILES
        include/${PROJECT_NAME}/*.h
        src/*.h
        src/*.cpp)

if (BUILD_SHARED_LIBS)
    add_compile_definitions(PB_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(workbench)

if(APPLE)
    set(CONVERTER_POSTFIX "mac.sh")
    set(SHARED_LIB_NAME "lib${PROJECT_NAME}.dylib")
elseif(WIN32) 
    set(CONVERTER_POSTFIX "win.bat")
    set(SHARED_LIB_NAME "${PROJECT_NAME}.dll")
else()
    set(CONVERTER_POSTFIX "linux.sh")
    set(SHARED_LIB_NAME "lib${PROJECT_NAME}.so")
endif()

if (PB_BUILD_EXAMPLES)
    add_subdirectory(samples/Sandbox)

    if (BUILD_SHARED_LIBS)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            DEPENDS Sandbox
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} samples/Sandbox/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
            COMMENT "Copy library"
    )
  endif()
endif()



# Hack until vcpkg fixes physx setup

if(APPLE)
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-osx/$<$<CONFIG:Debug>:debug>/lib)
    
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
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/$<$<CONFIG:Debug>:debug>/lib)

    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        PhysXPvdSDK_static_64
        PhysXExtensions_static_64
        PhysXFoundation_64
        PhysXCommon_64
        PhysX_64
    )
else()
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/$<$<CONFIG:Debug>:debug>/lib)

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

    

# ---------------- EXTERN ---------------------#

set(EXTERN_DIR extern/)

# glad
add_subdirectory(${EXTERN_DIR}glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)


# VCPKG INCLUDES

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# freetype
find_package(freetype CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

# ingui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

# zlib
find_package(ZLIB REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

# physx
#find_package(physx CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PUBLIC physx)



set_target_properties(glad PROPERTIES FOLDER ${EXTERN_DIR})