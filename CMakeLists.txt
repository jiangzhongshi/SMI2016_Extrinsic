cmake_minimum_required (VERSION 3.1)
project (extrinsic)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /bigobj /w") ### Enable parallel compilation
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR} )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR} )
else()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2") #### Libigl requires a modern C++ compiler that supports c++11
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -Wno-unused-parameter -Wno-deprecated-register -Wno-return-type-c-linkage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations -Wno-unused-parameter -Wno-deprecated-register -Wno-return-type-c-linkage")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Os")
endif()


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  # set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    # "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

if(APPLE)
  # Try to auto-detect a suitable SDK
  execute_process(COMMAND bash -c "xcodebuild -version -sdk | grep MacOSX | grep Path | head -n 1 | cut -f 2 -d ' '" OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
  string(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
  string(REGEX REPLACE "^.*X([0-9.]*).sdk$" "\\1" CMAKE_OSX_DEPLOYMENT_TARGET "${CMAKE_OSX_SYSROOT}")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Enable C++11 mode on GCC / Clang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# Compile with compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-anonymous-struct -Wno-c99-extensions -Wno-nested-anon-types")
  endif()
endif()

# Real Code starts here...
file(GLOB SRC_FILE src/*.cpp)

# add_executable(${PROJECT_NAME}_bin ${SRC_FILE})
# target_include_directories(${PROJECT_NAME}_bin PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/include 
# ${CMAKE_CURRENT_SOURCE_DIR}/)

add_subdirectory(pybind11)
add_library(extrinsic MODULE src_py/pybinding.cpp)
target_link_libraries(extrinsic PRIVATE pybind11::module)
target_include_directories(extrinsic PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include 
${CMAKE_CURRENT_SOURCE_DIR}/
${CMAKE_CURRENT_SOURCE_DIR}/eigen3
${PYTHON_INCLUDE_DIR})
set_target_properties(extrinsic PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                         SUFFIX "${PYTHON_MODULE_EXTENSION}")
