cmake_minimum_required(VERSION 2.6)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")


project(broadcast)




set(TARGET ${CMAKE_PROJECT_NAME})
set(LIBS lboost_system lboost_thread pthread)



find_package(Boost 1.58.0 COMPONENTS date_time filesystem system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(Boost_USE_STATIC_LIBS ON) # only find static libs
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


set(SOURCES src/main.cpp
        src/reliablebroadcast.cpp
        src/message.cpp
        src/node.cpp
        src/externalmessage.cpp src/internalmessage.cpp
        src/echomessage.cpp
        src/sendmessage.cpp
        src/readymessage.cpp
        src/socketcontroller.cpp
        src/session.cpp
        src/hashmessage.cpp)

set(HEADERS
        src/reliablebroadcast.h
        src/message.h
        src/node.h
        src/externalmessage.h
        src/internalmessage.h
        src/echomessage.h
        src/sendmessage.h
        src/readymessage.h
        src/socketcontroller.h
        src/session.h
        src/threadsafequeue.h
        src/hashmessage.h)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(broadcast ${SOURCES} ${HEADERS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()