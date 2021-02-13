project (ImGui CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(${PROJECT_NAME} STATIC imconfig.h imgui_demo.cpp imgui.cpp imgui.h imgui_draw.cpp imgui_internal.h imgui_tables.cpp imgui_widgets.cpp imstb_truetype.h imstb_textedit.h imstb_rectpack.h)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
