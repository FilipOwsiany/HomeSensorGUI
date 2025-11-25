cmake_minimum_required(VERSION 3.10)
project(backend C CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # *.a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # *.so
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # executables

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CROSSCOMPILING)
    set(CMAKE_C_FLAGS_DEBUG   "-g -Og")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
else()
    set(CMAKE_C_FLAGS_DEBUG   "-g -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

add_compile_options(-Wall -Wextra -Wpedantic)
include_directories(${CMAKE_SOURCE_DIR})

set(APP_SOURCE_DIR src)
set(APP_INCLUDE_DIR inc)

set(MAIN_SOURCE_FILES
    ${APP_SOURCE_DIR}/main.cpp
    ${APP_SOURCE_DIR}/CJsonParser.cpp
    ${APP_SOURCE_DIR}/CBackend.cpp
)

set(MAIN_SOURCE_INCLUDES 
    ${APP_INCLUDE_DIR}
)

add_subdirectory(../shared/ipcSocket ipcSocket)

if(CMAKE_CROSSCOMPILING)
    message("************************* INFO **************************")
    message("Building backend for RPI5")

    add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILDING_FOR_RPI5=1)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty
        ${MAIN_SOURCE_INCLUDES}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread
        ipcSocket
        ssl
        crypto
    )

else()
    message("************************* INFO **************************")
    message("Building backend for Host")

    add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILDING_FOR_PC=1)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty
        ${MAIN_SOURCE_INCLUDES}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread
        ipcSocket
        ssl
        crypto
    )
endif()
