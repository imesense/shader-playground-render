# Metaproject 
project(RenderDirectX2DShaders
    VERSION ${SPG_VERSION}
)

add_custom_command(
    COMMAND ${FXC_COMMAND}
        /nologo
        /E main
        /T cs_5_0
        ${SPG_HLSL_FLAGS}
        /Zi
        /I ${CMAKE_CURRENT_SOURCE_DIR}
        /Fo "${CMAKE_CURRENT_BINARY_DIR}/compute.cso"
        /Fd "${CMAKE_CURRENT_BINARY_DIR}/compute.pdb"
        ${CMAKE_CURRENT_SOURCE_DIR}/compute.hlsl
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/compute.cso
    COMMENT "Compiling compute.hlsl..."
    VERBATIM
)
add_custom_command(
    COMMAND ${FXC_COMMAND}
        /nologo
        /E main
        /T ps_5_0
        ${SPG_HLSL_FLAGS}
        /Zi
        /Fo "${CMAKE_CURRENT_BINARY_DIR}/pixel.cso"
        /Fd "${CMAKE_CURRENT_BINARY_DIR}/pixel.pdb"
        ${CMAKE_CURRENT_SOURCE_DIR}/pixel.hlsl
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pixel.cso
    COMMENT "Compiling pixel.hlsl..."
    VERBATIM
)
add_custom_command(
    COMMAND ${FXC_COMMAND}
        /nologo
        /E main
        /T vs_5_0
        ${SPG_HLSL_FLAGS}
        /Zi
        /Fo "${CMAKE_CURRENT_BINARY_DIR}/vertex.cso"
        /Fd "${CMAKE_CURRENT_BINARY_DIR}/vertex.pdb"
        ${CMAKE_CURRENT_SOURCE_DIR}/vertex.hlsl
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vertex.cso
    COMMENT "Compiling vertex.hlsl..."
    VERBATIM
)

add_custom_target(${PROJECT_NAME}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/compute.cso
        ${CMAKE_CURRENT_BINARY_DIR}/pixel.cso
        ${CMAKE_CURRENT_BINARY_DIR}/vertex.cso
)

# Project for DirectX 2D render
project(RenderDirectX2D
    LANGUAGES CXX
    VERSION ${SPG_VERSION}
)

set(SPG_RENDER_DX2D_BUILD_DEFINES
    _MBCS
    RENDER_DIRECTX_2D
)
set(SPG_RENDER_DX2D_BUILD_FLAGS
    ${SPG_CXX_FLAGS}
)
set(SPG_RENDER_DX2D_LINK_FLAGS
    ${SPG_LINKER_FLAGS}
)

set(SPG_RENDER_DX2D_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
set(SPG_RENDER_DX2D_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Exports.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SharedConst.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StringUtils.hpp
)

set(SHADER_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
string(REPLACE "/" "\\\\" SHADER_OUTPUT_PATH ${SHADER_OUTPUT_PATH})
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
)
set(SPG_RENDER_DX2D_RESOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/resource.rc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED
    ${SPG_RENDER_DX2D_SOURCES}
    ${SPG_RENDER_DX2D_INCLUDES}
    ${SPG_RENDER_DX2D_RESOURCES}
)
add_dependencies(${PROJECT_NAME} RenderDirectX2DShaders)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.hpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${SPG_RENDER_DX2D_BUILD_DEFINES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_DX2D_BUILD_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_DX2D_LINK_FLAGS})

target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE d3dcompiler.lib)
