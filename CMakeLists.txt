cmake_minimum_required(VERSION 2.6)

project(broadcast)

set (CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

find_package(Boost 1.58.0 COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} pthread)
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()
