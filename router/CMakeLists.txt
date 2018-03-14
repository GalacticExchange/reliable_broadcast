cmake_minimum_required(VERSION 2.6)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")


project(router)


set(TARGET ${CMAKE_PROJECT_NAME})
set(LIBS lboost_system lboost_thread pthread)


find_package(Boost 1.58.0 COMPONENTS date_time filesystem system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS ON) # only find static libs
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)


if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(router ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()