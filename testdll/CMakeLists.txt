# cmake cpp demo 
cmake_minimum_required(VERSION 3.25)

# project name
project(testdll)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

set(VCPKG_TARGET_TRIPLET "x64-windows")

list(APPEND INCLUDES 
    ${PROJECT_SOURCE_DIR}/include
    ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include
)

message(STATUS "INCLUDES: ${INCLUDES}")

# windows export all symbols
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# fmt 
find_package(fmt REQUIRED)

add_library(staticlib STATIC ${PROJECT_SOURCE_DIR}/src/staticlib.cpp)
target_link_libraries(staticlib PUBLIC fmt::fmt)
target_include_directories(staticlib PUBLIC ${INCLUDES})

# add library
add_library(dll SHARED ${PROJECT_SOURCE_DIR}/src/dll.cpp)
target_link_libraries(dll PUBLIC staticlib)
target_include_directories(dll PUBLIC ${INCLUDES})

add_executable(testdll ${PROJECT_SOURCE_DIR}/src/main.cpp
                    ${PROJECT_SOURCE_DIR}/src/testsingle.cpp
)
target_link_libraries(testdll PRIVATE dll)
target_include_directories(testdll PUBLIC ${INCLUDES})

