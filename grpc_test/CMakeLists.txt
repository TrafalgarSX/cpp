cmake_minimum_required(VERSION 3.18) 

project(grpc_test VERSION 1.0.0)

message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#print some message  eg. variable 
message(CMAKE_CXX_STANDARD="${CMAKE_CXX_STANDARD}")

set(MY_DEV_LIB_PATH "D:/workspace/dev")

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
                           "${PROJECT_SOURCE_DIR}/generate" # 本工程的头文件
                           "${MY_DEV_LIB_PATH}/include" # 第三方库的头文件
                           )

list(APPEND EXTRA_SRC
                      "${PROJECT_SOURCE_DIR}/src/grpc_test.cpp"
    )

#link lib 后面应该完善，操作系统和编译器组合判断
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message("windows")
# 根据编译器的不同，设置不同的编译选项和依赖库
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("mingw")
    # 方便生成compile_commands.json
    list(APPEND EXTRA_INCLUDE "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows/include")
    list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/unixlib")
    list(APPEND EXTRA_COMPILE_FLAGS "-Wall")

    list(APPEND EXTRA_LIB gtest pthread)

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message("clang")

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message("msvc")
    # set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "D:/workspace/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_PREFIX_PATH "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows")
    set(VCPKG_FEATURE_FLAGS "versions")

    # 第三方库的头文件（vcpkg安装的）
    list(APPEND EXTRA_INCLUDE "${CMAKE_PREFIX_PATH}/include")
    # 第三方库的路径（vcpkg安装的）
    # list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin")
    # 区分debug和release, 多配置编译器不能直接使用CMAKE_BUILD_TYPE
    list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}-static/$<$<CONFIG:Debug>:debug/>lib")
    list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>lib")
    # 自己编译的库的路径(msvc编译器)
    list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/msvclib")

    # list(APPEND EXTRA_DEFINES "_LOG_")
    # list(APPEND EXTRA_COMPILE_FLAGS "$<IF:$<CONFIG:Debug>,MTd,MT>")
    list(APPEND EXTRA_COMPILE_FLAGS "/MD$<$<CONFIG:Debug>:d>"  "/utf-8")

    # 动态库
    find_package(fmt CONFIG REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(absl CONFIG REQUIRED)
    find_package(protobuf CONFIG REQUIRED)
    find_package(gRPC CONFIG REQUIRED)

    list(APPEND EXTRA_LIB fmt::fmt )
    list(APPEND EXTRA_LIB protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
    list(APPEND EXTRA_LIB gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts gRPC::grpc++_reflection)
    list(APPEND EXTRA_LIB absl::any absl::base)

  endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message("unix")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/android-lib/${ANDROID_ABI}")
  list(APPEND EXTRA_LIB curl)
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

# Proto file
# get_filename_component(hw_proto "${PROJECT_SOURCE_DIR}/protos/helloworld.proto" PATH)
# get_filename_component(hw_proto_path "${PROJECT_SOURCE_DIR}/protos/helloworld.proto" PATH)

set(hw_proto "${PROJECT_SOURCE_DIR}/protos/helloworld.proto")
set(hw_proto_path "${PROJECT_SOURCE_DIR}/protos/")

set(hw_proto_srcs "${PROJECT_SOURCE_DIR}/generate/helloworld.pb.cc")
set(hw_proto_hdrs "${PROJECT_SOURCE_DIR}/generate/helloworld.pb.h")
set(hw_grpc_srcs "${PROJECT_SOURCE_DIR}/generate/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${PROJECT_SOURCE_DIR}/generate/helloworld.grpc.pb.h")

set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
message("_PROTOBUF_PROTOC :${_PROTOBUF_PROTOC}")
message("_GRPC_CPP_PLUGIN_EXECUTABLE :${_GRPC_CPP_PLUGIN_EXECUTABLE}")

add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROJECT_SOURCE_DIR}/generate"
        --cpp_out "${PROJECT_SOURCE_DIR}/generate"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# protobuf_generate_cpp(hw_proto_srcs hw_proto_hdrs "${PROJECT_SOURCE_DIR}/protos/helloworld.proto")
# protobuf_generate_cpp(hw_grpc_srcs hw_grpc_hdrs "${PROJECT_SOURCE_DIR}/protos/helloworld.proto")

add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})

target_link_libraries(hw_grpc_proto PRIVATE common)

foreach(_target greeter_client greeter_server
  greeter_async_client greeter_async_server
  greeter_async_client2)
  add_executable(${_target}  "${PROJECT_SOURCE_DIR}/src/${_target}.cpp")
  target_link_libraries(${_target} PRIVATE common hw_grpc_proto)
endforeach()


add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE common)
# 源文件
target_sources(${PROJECT_NAME} PUBLIC ${EXTRA_SRC})

# copy need multi dll to exe dir 
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/fmt$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/libprotobuf$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/libprotobuf-lite$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/libprotoc$<$<CONFIG:Debug>:d>.dll"
    "${CMAKE_PREFIX_PATH}/bin/zlib1.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/cares.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/re2.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/abseil_dll.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/libssl-3-x64.dll"
    "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin/libcrypto-3-x64.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)

include(CTest)

# add_test(NAME first COMMAND gtest_server)
# add_test(NAME second COMMAND gtest_client)

