cmake_minimum_required( VERSION 3.2 )

project(lvgl)

message(${PROJECT_SOURCE_DIR})

set (CMAKE_CXX_COMPILER g++)
set (CMAKE_C_COMPILER   gcc)

add_definitions(-DLV_CONF_INCLUDE_SIMPLE) 

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/tmp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/tmp)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/lvgl_porting SRC_LVGL)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LVGL)

add_executable(test ${SRC_LVGL} ${PROJECT_SOURCE_DIR}/src/main.c ${PROJECT_SOURCE_DIR}/src/png_to_rgb.c)

target_include_directories(
    test 
    PRIVATE
    ${PROJECT_SOURCE_DIR}/third/lvgl
    ${PROJECT_SOURCE_DIR}/third/libpng
    ${PROJECT_SOURCE_DIR}/src/lvgl_porting
    ${PROJECT_SOURCE_DIR}/src
)

target_link_directories(
    test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/tmp
)

target_link_libraries(
    test
    PRIVATE
    lvgl png
)
