cmake_minimum_required(VERSION 2.6)

project(broadcast)

add_subdirectory(router)
set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)
file(GLOB ROUTER_FILES
        router/node_config.h
        router/node_config.cpp
        )

set(LIBS cpp_redis tacopie)

find_package(Boost 1.58.0 COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${ROUTER_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBS})
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()
