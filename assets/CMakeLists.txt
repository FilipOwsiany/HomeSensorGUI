add_library(assets SHARED 
        icons/src/garlic.c 
        icons/src/home.c
        icons/src/information.c
        icons/src/layers.c
        icons/src/back.c
        icons/src/settings.c 
        icons/src/bell.c
        icons/src/cloud.c
        icons/src/wifi.c
        icons/src/bluetooth.c
        fonts/src/AlanSans_20_Medium.c
        fonts/src/AlanSans_32_Medium.c
        )

if(CMAKE_CROSSCOMPILING)
    target_include_directories(assets
        PUBLIC
            ${CMAKE_SOURCE_DIR}/lvgl
            ${CMAKE_SOURCE_DIR}/config_rpi5
            ${CMAKE_SOURCE_DIR}
    )
else()
    target_include_directories(assets
        PUBLIC
            ${CMAKE_SOURCE_DIR}/lvgl
            ${CMAKE_SOURCE_DIR}/config_pc
            ${CMAKE_SOURCE_DIR}
    )
endif()


