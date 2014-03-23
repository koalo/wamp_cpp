cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(wampcpp_demo)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_executable(wampcpp_demo wampcpp_demo.cpp DemoServer.cpp)

find_path(WAMP_CPP_DIR
  NAMES RPCallable.h
  PATHS ./
)

include_directories(${WAMP_CPP_DIR})
target_link_libraries(wampcpp_demo ${WAMP_CPP_DIR}/libwamp_cpp.a)

find_package(Boost 1.40 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(wampcpp_demo ${Boost_LIBRARIES})

find_package(JsonCpp REQUIRED)
include_directories(${JsonCpp_INCLUDE_DIR})
target_link_libraries(wampcpp_demo ${Jsoncpp_LIBRARY})

set(websocketpp_DIR "../websocketpp/lib/cmake/websocketpp/")
find_package(websocketpp REQUIRED)
include_directories(${WEBSOCKETPP_INCLUDE_DIR})

INSTALL(TARGETS wampcpp_demo
	RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
