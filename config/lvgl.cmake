cmake_minimum_required( VERSION 3.2 )

project(lvgl)

message(${PROJECT_SOURCE_DIR})

set (CMAKE_CXX_COMPILER g++)
set (CMAKE_C_COMPILER   gcc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_core SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_draw SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_font SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_gpu SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_hal SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_misc SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_themes SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lv_widgets SRC_LIST)

message("=====================")
message(${SRC_LIST})
message("=====================")

add_library(lvgl STATIC ${SRC_LIST})

target_include_directories(
    lvgl 
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/lv_core
    ${PROJECT_SOURCE_DIR}/src/lv_draw
    ${PROJECT_SOURCE_DIR}/src/lv_font
    ${PROJECT_SOURCE_DIR}/src/lv_gpu
    ${PROJECT_SOURCE_DIR}/src/lv_hal
    ${PROJECT_SOURCE_DIR}/src/lv_misc
    ${PROJECT_SOURCE_DIR}/src/lv_themes
    ${PROJECT_SOURCE_DIR}/src/lv_widgets
)
