cmake_minimum_required(VERSION 3.18)

project(ShaderPlayground.Render)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SKIP_RPATH FALSE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(SPG_VERSION "0.1.0")

set(SPG_CXX_FLAGS
    $<$<CXX_COMPILER_ID:MSVC>:
        /MP
        /W3
        /sdl
        /permissive-
        $<$<CONFIG:Debug>:
            /Od
            /MDd
        >
        $<$<CONFIG:RelWithDebInfo>:
            /O2
            /MD
            /GL
        >
        $<$<CONFIG:Release>:
            /O2
            /MD
            /GL
        >
    >
)
set(SPG_LINKER_FLAGS
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:RelWithDebInfo>:
            /INCREMENTAL:NO
            /LTCG
            /OPT:REF
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /INCREMENTAL:NO
            /LTCG
            /OPT:REF
            /OPT:ICF
        >
    >
)
set(SPG_HLSL_FLAGS
    $<IF:$<CONFIG:DEBUG>,/Od,/O3>
)

find_program(FXC_EXECUTABLE fxc.exe
    HINTS
        "$ENV{ProgramFiles\(x86\)}/Windows Kits/10/bin/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}"
    PATH_SUFFIXES
        x64
        x86
)
if(NOT FXC_EXECUTABLE)
    message(WARNING "fxc.exe not found. Shader compilation may fail.")
    set(FXC_EXECUTABLE "fxc.exe")
endif()

add_subdirectory(src/RenderDirectX2D)
add_subdirectory(src/RenderTestApp2D)
