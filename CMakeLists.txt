cmake_minimum_required (VERSION 3.1)
project (extrinsic)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB SRC_FILE src/*.cpp)

add_executable(${PROJECT_NAME}_bin ${SRC_FILE})
target_include_directories(${PROJECT_NAME}_bin PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/)
