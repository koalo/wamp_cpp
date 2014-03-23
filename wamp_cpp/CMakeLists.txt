cmake_minimum_required(VERSION 2.6)

project(wamp_cpp)

SET(CMAKE_CXX_FLAGS "-std=c++0x")
add_library (wamp_cpp WAMPServer.cpp MessageHandler.cpp Directory.cpp Json.cpp EventManager.cpp Topic.cpp)

set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set (DEF_INSTALL_CMAKE_DIR lib/cmake/wamp_cpp)
set (INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

find_package(websocketpp REQUIRED)
include_directories(${WEBSOCKETPP_INCLUDE_DIR})
message(${WEBSOCKETPP_INCLUDE_DIR})
#target_link_libraries(wamp_cpp ${websocketpp_LIBRARY})

export(PACKAGE wamp_cpp)

configure_file (wamp_cpp-config.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wamp_cpp-config.cmake" @ONLY)

install (FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/wamp_cpp-config.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)


