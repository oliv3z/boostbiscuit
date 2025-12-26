
cmake_minimum_required(VERSION 3.30)
project(boostbiscuit)

set(CMAKE_CXX_STANDARD 20)

# JAVA (for JNI, JVMTI, etc.)
set(JAVA_HOME "C:/Program Files/Java/jdk1.8.0_202")  # TODO: change when release
include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/win32)
link_directories(${JAVA_HOME}/lib)

# MINHOOK
set(MINHOOK_SOURCE_DIR ${PROJECT_SOURCE_DIR}/include/minhook)
file(GLOB MINHOOK_SOURCES "${MINHOOK_SOURCE_DIR}/src/*.c" "${MINHOOK_SOURCE_DIR}/src/hde/*.c" "${MINHOOK_SOURCE_DIR}/src/hde/hde64.c")
add_library(minhook STATIC ${MINHOOK_SOURCES})

# MinGW specific settings for MinHook
if(MINGW)
    target_compile_definitions(minhook PRIVATE _WIN32_WINNT=0x0600)
endif()

# IMGUI
set(IMGUI_SOURCE_DIR ${PROJECT_SOURCE_DIR}/include/imgui)
file(GLOB IMGUI_SOURCES "${IMGUI_SOURCE_DIR}/*.cpp" "${IMGUI_SOURCE_DIR}/*.h")
add_library(imgui STATIC ${IMGUI_SOURCES})

# Copy resource files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/)

# Add source files in src directory
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

add_library(boostbiscuit SHARED ${SRC_FILES} ${IMGUI_SOURCES}
        src/utils/structs/vec3.h
        src/utils/render.h
        src/utils/structs/vec2.h
        src/utils/structs/vec4.h
        src/wrapper/minecraft/util/Timer.cpp
        src/wrapper/minecraft/util/Timer.h
        src/hooks/detours/java/updateRenderInfo.cpp
        src/hooks/detours/java/updateRenderInfo.h
        src/features/module/module_manager.cpp
        src/features/module/module_manager.h
        src/features/module/bb_module.h
        src/features/module/modules/render/esp.cpp
        src/features/module/modules/render/esp.h
        src/features/module/common_data.h
        src/features/module/other/render_data.cpp
        src/features/module/other/render_data.h
        include/imgui/custom/custom.hpp
        include/imgui/custom/widgets/checkbox.cpp
        include/imgui/custom/widgets/text_button.cpp
        include/imgui/custom/widgets/navbar.cpp
        include/imgui/custom/widgets/child.cpp
        include/imgui/fonts/GeistMedium.h
        include/imgui/custom/widgets/combo.cpp
        include/imgui/custom/widgets/selectable.cpp
        include/imgui/custom/widgets/color_edit.cpp
        include/imgui/custom/widgets/keybind.cpp
        include/imgui/custom/widgets/off_on_button.cpp
        src/wrapper/minecraft/client/gui/GuiScreen.cpp
        src/wrapper/minecraft/client/gui/GuiScreen.h
        include/imgui/custom/widgets/slider.cpp
        src/features/module/register/class_register.cpp
        src/features/module/register/class_register.h
        src/hooks/detours/java/renderName_detour.cpp
        src/hooks/detours/java/renderName_detour.h
        src/features/module/modules/render/nametags.cpp
        src/features/module/modules/render/nametags.h)

set_target_properties(boostbiscuit PROPERTIES OUTPUT_NAME "boostbiscuit")
set_target_properties(boostbiscuit PROPERTIES PREFIX "")

# Link against some libraries
find_package(OpenGL REQUIRED)
target_link_libraries(boostbiscuit jvm user32.lib minhook OpenGL::GL dwmapi)

# Make #include path start from project root (no ../../../../)
target_include_directories(boostbiscuit PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include/minhook/include ${IMGUI_SOURCE_DIR})
