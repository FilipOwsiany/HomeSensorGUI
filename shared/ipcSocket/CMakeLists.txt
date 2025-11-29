cmake_minimum_required(VERSION 3.10)
project(ipcSocketLib CXX)

if (NOT TARGET logger)
    set(LOGGER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../logger")

    if (EXISTS "${LOGGER_SRC_DIR}/CMakeLists.txt")
        message(STATUS "ipcSocket: adding logger from ${LOGGER_SRC_DIR}")
        add_subdirectory("${LOGGER_SRC_DIR}" logger_from_ipcSocket)
    else()
        message(FATAL_ERROR
            "Target 'logger' not found and ${LOGGER_SRC_DIR}/CMakeLists.txt does not exist.\n"
        )
    endif()
endif()

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

target_link_libraries(ipcSocket
    PRIVATE
        logger
)

target_include_directories(ipcSocket
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Events
)
