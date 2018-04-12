cmake_minimum_required(VERSION 2.6)

project(broadcast)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread -Wall")

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)
#set (ROUTER_SOURCES
#    "src/file_utils.cpp"
#)

set(LIBS cpp_redis tacopie)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58.0 COMPONENTS system thread log REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
#    add_executable(${PROJECT_NAME} ${SOURCES} ${ROUTER_SOURCES} ${HEADERS})
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBS})
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()
