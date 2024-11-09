cmake_minimum_required(VERSION 3.29) 

project(test_module VERSION 1.0.0)

#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(ARCH_TYPE "${CMAKE_SYSTEM_PROCESSOR}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(ARCH_TYPE "${CMAKE_SYSTEM_PROCESSOR}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  set(ARCH_TYPE "${CMAKE_ANDROID_ARCH_ABI}")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message("64bit")
  set(BUILD_BIT "x64")
else()
  # 暂时用不到32位
  message("32bit")
  set(BUILD_BIT "x86")
endif()


#static lib output path  windows: .a .lib   unix: .a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/lib")
# 目标链接库的存放位置 unix: .so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/lib")
# windows: .exe .dll   unix: 可执行文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/bin")

# 通用宏定义
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

list(APPEND EXTRA_SRC "${PROJECT_SOURCE_DIR}/src/test_module.cpp")

#link lib 后面应该完善，操作系统和编译器组合判断
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # list(APPEND EXTRA_COMPILE_FLAGS "$<IF:$<CONFIG:Debug>,MTd,MT>")
    list(APPEND EXTRA_COMPILE_FLAGS "/MD$<$<CONFIG:Debug>:d>")
    # 动态库
    find_package(fmt CONFIG REQUIRED)

    list(APPEND EXTRA_LIB fmt::fmt)
  endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message("unix")
endif()
message("EXTRA_COMPILE_FLAGS: ${EXTRA_COMPILE_FLAGS}")
message("EXTRA_SRC: ${EXTRA_SRC}")

add_library(common INTERFACE)

# target_link_options(${PROJECT_NAME} PUBLIC ${EXTRA_LINK_FLAGS})
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


add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE common)

list(APPEND MODULE_SET "${PROJECT_SOURCE_DIR}/src/helloworld.ixx")

# 源文件
target_sources(${PROJECT_NAME} PUBLIC ${EXTRA_SRC})

# 添加模块文件
target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET all_my_modules TYPE CXX_MODULES FILES
    ${MODULE_SET}
)