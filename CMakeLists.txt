cmake_minimum_required(VERSION 3.5)
project(HoMM3UnitHelper VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

option(CMAKE_BUILD_TYPE "Build type" debug)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/)

# Build for windows
add_compile_definitions(WIN32)

option(UNICODE "Build using wide char WinAPI" ON)
if(UNICODE)
    add_compile_definitions(UNICODE)
    #ImGui uses _T macro
    add_compile_definitions(_UNICODE)
endif()

if(NOT CMAKE_HOST_WIN32)
    set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

    # MinGW libs directory
    if(EXISTS /usr/x86_64-w64-mingw32/lib/)
        link_directories(HoMM3UnitHelper /usr/x86_64-w64-mingw32/lib)
    elseif(EXISTS /usr/i686-w64-mingw32/lib/)
        link_directories(HoMM3UnitHelper /usr/i686-w64-mingw32/lib)
    endif()
endif()

# ImGui library
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImGui)

set(IMGUI_HEADERS
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/backends/imgui_impl_win32.h
    ${IMGUI_DIR}/backends/imgui_impl_dx9.h
)

set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx9.cpp
)

add_library(ImGui STATIC ${IMGUI_HEADERS} ${IMGUI_SRC})
target_link_libraries(ImGui PRIVATE d3d9 dwmapi)

target_sources(ImGui
    PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

target_include_directories(ImGui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

# App def
if(MINGW)
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    #SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -i Homm3UnitHelper.rc -o Homm3UnitHelper.res")
endif()

set(SOURCES
    src/DataReader.cpp
    src/MemoryReader.cpp
    src/Unit.cpp
    Homm3UnitHelper.rc
)

add_executable(HoMM3UnitHelper WIN32 main.cpp ${SOURCES} resource.h)

target_link_libraries(HoMM3UnitHelper ImGui d3d9 urlmon)

target_include_directories(HoMM3UnitHelper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

