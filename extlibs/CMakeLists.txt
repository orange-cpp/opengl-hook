project(imgui LANGUAGES CXX)

add_library(imgui STATIC imgui/imgui.cpp)
target_include_directories(imgui PUBLIC imgui imgui/backends)
target_sources(imgui PRIVATE
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_win32.cpp)


add_subdirectory(minhook)