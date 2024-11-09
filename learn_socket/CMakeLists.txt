cmake_minimum_required(VERSION 3.22)
project(learn_socket)

# set cxx standard 
set(CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_STANDARD 11)

list(APPEND lib_src 
                    ${PROJECT_SOURCE_DIR}/src/hex_util.cpp
                   )

list(APPEND exe_src 
                   ${PROJECT_SOURCE_DIR}/src/main.cpp
                   )

list(APPEND include_dir ${PROJECT_SOURCE_DIR}/include
                   )

add_library(${PROJECT_NAME}s STATIC ${lib_src})
target_include_directories(${PROJECT_NAME}s PRIVATE ${include_dir})

add_executable(${PROJECT_NAME} ${exe_src})
target_include_directories(${PROJECT_NAME} PRIVATE ${include_dir})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}s ws2_32.lib)
