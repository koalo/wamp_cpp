cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(ExternalProject)

set(websocketpp_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/websocketpp")
set(websocketpp_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${websocketpp_INSTALL_DIR})

ExternalProject_Add(websocketpp
  GIT_REPOSITORY https://github.com/zaphoyd/websocketpp
  INSTALL_DIR ${websocketpp_INSTALL_DIR}
  CMAKE_ARGS ${websocketpp_CMAKE_ARGS}
)

set(wamp_cpp_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/wamp_cpp")
set(wamp_cpp_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/wamp_cpp")
set(wamp_cpp_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/wamp_cpp")
set(wamp_cpp_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${wamp_cpp_INSTALL_DIR})

ExternalProject_Add(wamp_cpp
  DEPENDS websocketpp
  SOURCE_DIR ${wamp_cpp_SOURCE_DIR}
  BINARY_DIR ${wamp_cpp_BINARY_DIR}
  INSTALL_DIR ${wamp_cpp_INSTALL_DIR}
  CMAKE_ARGS ${wamp_cpp_CMAKE_ARGS}
)

ExternalProject_Add(wampcpp_demo
  DEPENDS wamp_cpp
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/wampcpp_demo"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/wampcpp_demo"
  INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/wampcpp_demo"
)
