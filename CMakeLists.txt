
#   cmake -B build -S .
#   cmake --build build --config Debug
#   cmake --build build --config Release
#   ./build/Debug/app.exe

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

add_executable(app 
    main.cpp
    gui.cpp
    esp.cpp
    ${IMGUI_SRC}
)

target_include_directories(app PRIVATE
    ${IMGUI_DIR}
    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(app PRIVATE 
    d3d9
    gdi32    
    ws2_32
)

set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)