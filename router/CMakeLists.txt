cmake_minimum_required(VERSION 2.6)

set (CMAKE_CXX_STANDARD 11)

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

file(GLOB SRC_FILES
        ../src/message.h
        ../src/message.cpp

#        ../src/internalmessage.h
#        ../src/internalmessage.cpp
#        ../src/externalmessage.h
#        ../src/externalmessage.cpp
#        ../src/sendmessage.h
#        ../src/sendmessage.cpp
#        ../src/echomessage.h
#        ../src/echomessage.cpp
#        ../src/readymessage.h
#        ../src/readymessage.cpp
#        ../src/hashmessage.h
#        ../src/hashmessage.cpp
#        ../src/session.h
#        ../src/session.cpp
#        ../src/reliablebroadcast.h
#        ../src/reliablebroadcast.cpp
#        ../src/node.h
#        ../src/node.cpp
#        ../src/messagelistener.h
#        ../src/messagelistener.cpp
        )


set(LIBS pthread)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(router ${SOURCES} ${HEADERS} ${SRC_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBS})
else ()
    message(SEND_ERROR "Failed to find boost.")
    return()
endif ()