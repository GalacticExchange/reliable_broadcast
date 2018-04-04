cmake_minimum_required(VERSION 2.6)

project(broadcast)

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

set(LIBS cpp_redis tacopie)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58.0 COMPONENTS system thread log REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBS})
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()
