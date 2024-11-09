cmake_minimum_required(VERSION 3.20) 

project(zhcosign VERSION 1.0.0)

#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#print some message  eg. variable 
message(CMAKE_CXX_STANDARD="${CMAKE_CXX_STANDARD}")

# 通用宏定义
list(APPEND EXTRA_LINK_FLAGS)
list(APPEND EXTRA_DEFINES)
# 编译选项 目前只支持gcc和clang, msvc用CMAKE_CXX_FLAGS
list(APPEND EXTRA_COMPILE_FLAGS)
# 依赖的库的路径
list(APPEND EXTRA_LIB_PATH)
# 依赖的库
list(APPEND EXTRA_LIB)

list(APPEND EXTRA_INCLUDE  
                           "${PROJECT_SOURCE_DIR}/include" # 本工程的头文件
                           )

list(APPEND EXTRA_SRC)

#link lib 后面应该完善，操作系统和编译器组合判断
# if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
if(0)
    message("msvc")
    # set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "D:/workspace/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_PREFIX_PATH "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows")
    set(VCPKG_FEATURE_FLAGS "versions")

    # 第三方库的头文件（vcpkg安装的）
    list(APPEND EXTRA_INCLUDE "${CMAKE_PREFIX_PATH}/include")
    list(APPEND EXTRA_INCLUDE "D:/workspace/dev/vcpkg/packages/sciter-js_x64-windows/include")
    # 第三方库的路径（vcpkg安装的）
    # list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin")
    # 区分debug和release, 多配置编译器不能直接使用CMAKE_BUILD_TYPE
    # list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>lib")
    # list(APPEND EXTRA_COMPILE_FLAGS "$<IF:$<CONFIG:Debug>,MTd,MT>")
    list(APPEND EXTRA_COMPILE_FLAGS "/MD$<$<CONFIG:Debug>:d>" "/utf-8")


    # 动态库
    # find_package(fmt CONFIG REQUIRED)
    # list(APPEND EXTRA_LIB fmt::fmt )
endif()

add_library(common INTERFACE)

target_link_options(common INTERFACE ${EXTRA_LINK_FLAGS})
# 编译选项
target_compile_options(common INTERFACE ${EXTRA_COMPILE_FLAGS})
# 宏定义
target_compile_definitions(common INTERFACE ${EXTRA_DEFINES})
# 工程下的头文件的路径
target_include_directories(common INTERFACE ${EXTRA_INCLUDE})
# 第三方库的路径
target_link_directories(common INTERFACE ${EXTRA_LIB_PATH})
# 第三方库
target_link_libraries(common INTERFACE ${EXTRA_LIB})


list(APPEND EXTRA_SRC "${PROJECT_SOURCE_DIR}/src/etcryptmedia_module.cpp"
                      "${PROJECT_SOURCE_DIR}/src/EtZHCosignTest.cpp"
)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE common)
# 源文件
target_sources(${PROJECT_NAME} PUBLIC ${EXTRA_SRC})

