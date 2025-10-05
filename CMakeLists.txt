cmake_minimum_required(VERSION 3.10)
project(displayApplication C CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_DEBUG   "-g -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")



add_compile_options(-Wall -Wextra -Wpedantic)
include_directories(${CMAKE_SOURCE_DIR})

set(PROJECT_SOURCE_DIR src)
set(PROJECT_INCLUDE_DIR inc)

set(LV_BUILD_USE_KCONFIG OFF CACHE BOOL "" FORCE)

set(MAIN_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/main.cpp

    ${PROJECT_SOURCE_DIR}/sidePanel/CSidePanel.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/button/CButton.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/object/CObject.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/cell/CCell.cpp
    ${PROJECT_SOURCE_DIR}/common/CCallbackInterface.cpp
    ${PROJECT_SOURCE_DIR}/mainViewManager/CMainViewManager.cpp
    ${PROJECT_SOURCE_DIR}/statusBar/CStatusBar.cpp
)

set(MAIN_SOURCE_INCLUDES 
    ${PROJECT_INCLUDE_DIR}

    ${PROJECT_INCLUDE_DIR}/sidePanel
    ${PROJECT_INCLUDE_DIR}/basicWidgets/button
    ${PROJECT_INCLUDE_DIR}/basicWidgets/object
    ${PROJECT_INCLUDE_DIR}/basicWidgets/cell
    ${PROJECT_INCLUDE_DIR}/common
    ${PROJECT_INCLUDE_DIR}/mainViewManager
    ${PROJECT_INCLUDE_DIR}/statusBar
)

if(CMAKE_CROSSCOMPILING)
    message("\n\nBuilding for RPI5\n\n")

    set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config_rpi5/lv_conf.h"
        CACHE FILEPATH "LVGL config file" FORCE)

    add_subdirectory(lvgl lvgl)
    add_subdirectory(assets)


    add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILDING_FOR_RPI5=1)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/config_rpi5
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${MAIN_SOURCE_INCLUDES}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE 
        lvgl 
        pthread 
        m
        assets
    )
else()
    message("\n\nBuilding for Host\n\n")

    set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config_pc/lv_conf.h"
        CACHE FILEPATH "LVGL config file" FORCE)

    add_subdirectory(lvgl lvgl)
    add_subdirectory(assets)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

    add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILDING_FOR_PC=1)

    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/config_pc
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${MAIN_SOURCE_INCLUDES}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        lvgl 
        pthread 
        m
        assets
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES}
    )
endif()
