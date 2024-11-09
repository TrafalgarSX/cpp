cmake_minimum_required(VERSION 3.29)

project(test_xlsreader)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    "C:/Qt/Qt5.9.0/5.9/mingw53_32/include/"
    "C:/Qt/Qt5.9.0/5.9/mingw53_32/include/QtCore"
    "C:/Qt/Qt5.9.0/5.9/mingw53_32/include/QtGui"
    "E:/work/QXlsx/QXlsx/header"
)
list(APPEND COMPILE_OPTIONS $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

list(APPEND DEFINES HAVE_ICONV)

list(APPEND LIB_HEADERS)

list(APPEND LIB_SRC
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/convert.cpp
)
if(MSVC)
    list(APPEND LIBS)
    list(APPEND DEFINES SPDLOG_HEADER_ONLY)
else()
    list(APPEND LIBS spdlog Qt5Cored QXlsx)
    list(APPEND DEFINES SPDLOG_COMPILED_LIB)
endif()

list(APPEND LIBS_DIR ${PROJECT_SOURCE_DIR}/lib
                     "C:/Qt/Qt5.9.0/5.9/mingw53_32/lib"
)

# set windows export all symbols 
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_executable(${PROJECT_NAME} ${LIB_HEADERS} ${LIB_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBS_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEFINES})

add_executable(watch_process ${PROJECT_SOURCE_DIR}/src/watch_process.cpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/Qt5Cored.dll"
        "${PROJECT_SOURCE_DIR}/lib/QXlsx.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)