cmake_minimum_required(VERSION 3.10)
project(displayApplication C CXX)

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

set(PROJECT_SOURCE_DIR src)
set(PROJECT_INCLUDE_DIR inc)

set(LV_BUILD_USE_KCONFIG OFF CACHE BOOL "" FORCE)

set(MAIN_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/main.cpp

    ${PROJECT_SOURCE_DIR}/CDisplayInitializer.cpp
    ${PROJECT_SOURCE_DIR}/CFrontend.cpp
    ${PROJECT_SOURCE_DIR}/CSegfaultHandler.cpp
    ${PROJECT_SOURCE_DIR}/CPirBacklightController.cpp

    ${PROJECT_SOURCE_DIR}/sidePanel/CSidePanel.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/button/CButton.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/object/CObject.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/cell/CCell.cpp
    ${PROJECT_SOURCE_DIR}/basicWidgets/infoBox/CInfoBox.cpp
    ${PROJECT_SOURCE_DIR}/common/CLVGLCallbackInterface.cpp
    ${PROJECT_SOURCE_DIR}/mainViewManager/CMainViewManager.cpp
    ${PROJECT_SOURCE_DIR}/statusBar/CStatusBar.cpp
    ${PROJECT_SOURCE_DIR}/styleFactory/CStyleFactory.cpp
    ${PROJECT_SOURCE_DIR}/views/sensorsView/CSensorsView.cpp
    ${PROJECT_SOURCE_DIR}/views/layoutView/CLayoutView.cpp
    ${PROJECT_SOURCE_DIR}/views/settingsView/CSettingsView.cpp
    ${PROJECT_SOURCE_DIR}/views/infoView/CInfoView.cpp
    ${PROJECT_SOURCE_DIR}/views/sensorsView/sensor/CSensor.cpp
)

set(MAIN_SOURCE_INCLUDES 
    ${PROJECT_INCLUDE_DIR}

    ${PROJECT_INCLUDE_DIR}/sidePanel
    ${PROJECT_INCLUDE_DIR}/basicWidgets/button
    ${PROJECT_INCLUDE_DIR}/basicWidgets/object
    ${PROJECT_INCLUDE_DIR}/basicWidgets/cell
    ${PROJECT_INCLUDE_DIR}/basicWidgets/infoBox
    ${PROJECT_INCLUDE_DIR}/common
    ${PROJECT_INCLUDE_DIR}/mainViewManager
    ${PROJECT_INCLUDE_DIR}/statusBar
    ${PROJECT_INCLUDE_DIR}/styleFactory
    ${PROJECT_INCLUDE_DIR}/views/sensorsView
    ${PROJECT_INCLUDE_DIR}/views/layoutView
    ${PROJECT_INCLUDE_DIR}/views/settingsView
    ${PROJECT_INCLUDE_DIR}/views/infoView
    ${PROJECT_INCLUDE_DIR}/views/sensorsView/sensor
)

if(CMAKE_CROSSCOMPILING)
    message("************************* INFO **************************")
    message("Building for RPI5")

    set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config_rpi5/lv_conf.h"
        CACHE FILEPATH "LVGL config file" FORCE)

    add_subdirectory(lvgl lvgl)
    add_subdirectory(assets)
    add_subdirectory(../shared/ipcSocket ipcSocket)


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
        ipcSocket
    )
else()
    message("************************* INFO **************************")
    message("Building for Host")

    set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config_pc/lv_conf.h"
        CACHE FILEPATH "LVGL config file" FORCE)

    add_subdirectory(lvgl lvgl)
    add_subdirectory(assets)
    add_subdirectory(../shared/ipcSocket ipcSocket)

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
        ipcSocket
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES}
    )
endif()
