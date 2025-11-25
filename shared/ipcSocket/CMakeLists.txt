cmake_minimum_required(VERSION 3.10)
project(ipcSocketLib CXX)

add_library(ipcSocket SHARED 
    CSocketServer.cpp
    CSocketClient.cpp
    CReceiver.cpp
    CSender.cpp
    CStream.cpp
    Events/CEvents.cpp
    Events/CEventBase.cpp
    Events/CEventDispatcher.cpp
)

target_include_directories(ipcSocket
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Events
)
