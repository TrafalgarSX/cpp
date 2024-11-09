# 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.25)

# 设置 vcpkg 工具链文件路径
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")
set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "Vcpkg target triplet")

# 设置项目名称
project(testVcpkg)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# 查找你需要的库
find_package(fmt CONFIG REQUIRED)

# 添加你的源文件
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)

# 链接你需要的库
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)