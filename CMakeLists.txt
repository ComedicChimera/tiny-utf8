cmake_minimum_required(
    VERSION 3.20.0
)

project(tiny_utf8)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(SRCS "reader.cpp")
list(TRANSFORM SRCS PREPEND "${SRC_DIR}/")

add_executable(${PROJECT_NAME} ${SRCS})

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
