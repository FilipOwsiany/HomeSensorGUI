add_library(ipcSocket STATIC 
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
