cmake_minimum_required(VERSION 3.20)
project(ir_remote)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_dx9.cpp
    ${IMGUI_DIR}/imgui_impl_win32.cpp
)

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "IMGUI_DIR = ${IMGUI_DIR}")
message(STATUS "IMGUI_SRC = ${IMGUI_SRC}")

find_package(CURL CONFIG REQUIRED)

add_executable(app 
    main.cpp
    gui.cpp
    esp.cpp
    ${IMGUI_SRC}
)

target_include_directories(app PRIVATE
    ${IMGUI_DIR}
)

target_link_libraries(app PRIVATE 
    CURL::libcurl
    d3d9
    gdi32    
)

set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)