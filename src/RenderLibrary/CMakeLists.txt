# Project for render library
project(RenderLibrary
    LANGUAGES CXX
    VERSION ${SPG_VERSION}
)

set(SPG_RENDER_LIBRARY_BUILD_DEFINES
    RENDERLIBRARY_EXPORTS
    _WINDOWS
    _USRDLL
    _UNICODE
)
set(SPG_RENDER_LIBRARY_BUILD_FLAGS
    ${SPG_CXX_FLAGS}
)
set(SPG_RENDER_LIBRARY_LINK_FLAGS
    ${SPG_LINKER_FLAGS}
)

set(SPG_RENDER_LIBRARY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DepthShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dllmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FabricDirectXWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FabricRender.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DX11ViewRender.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Exports.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Frustum.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputBinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Render.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShadowShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp
)
set(SPG_RENDER_LIBRARY_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DepthShader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FabricDirectXWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FabricRender.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DX11ViewRender.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Exports.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Frustum.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputBinder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputCodes.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FpsCounter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framework.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Helpers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputListener.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Light.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Memory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Render.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderState.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTarget.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Shader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShadowShader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.hpp
)
set(SPG_RENDER_LIBRARY_SHADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/depth.ps
    ${CMAKE_CURRENT_SOURCE_DIR}/depth.vs
    ${CMAKE_CURRENT_SOURCE_DIR}/shadow.ps
    ${CMAKE_CURRENT_SOURCE_DIR}/shadow.vs
)
set(SPG_RENDER_LIBRARY_IMAGES
    ${CMAKE_CURRENT_SOURCE_DIR}/ice.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/stone01.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/wall01.jpg
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../inc)

add_library(${PROJECT_NAME} SHARED
    ${SPG_RENDER_LIBRARY_SOURCES}
    ${SPG_RENDER_LIBRARY_INCLUDES}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${SPG_RENDER_LIBRARY_BUILD_DEFINES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_LIBRARY_BUILD_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_LIBRARY_LINK_FLAGS})

target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE winmm.lib)

foreach(SHADER_FILE ${SPG_RENDER_LIBRARY_SHADERS})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${SHADER_FILE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMENT "Copying shaders..."
    )
endforeach()
foreach(IMAGE_FILE ${SPG_RENDER_LIBRARY_IMAGES})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${IMAGE_FILE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMENT "Copying images..."
    )
endforeach()

add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${NUGET_COMMAND} restore
        "${CMAKE_CURRENT_SOURCE_DIR}/packages.config"
        -SolutionDirectory "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Restoring packages..."
)

# Nlohmann JSON
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/nlohmann.json.3.12.0/build/native/include)

# Microsoft Direct3DX
set(D3DX_DEBUG_POSTFIX "$<$<CONFIG:DEBUG>:d>")
set(D3DX_CONFIGURATION "$<IF:$<CONFIG:DEBUG>,debug,release>")
set(D3DX_BINARIES
    D3DCompiler_43.dll
    d3dx9${D3DX_DEBUG_POSTFIX}_43.dll
    d3dx10${D3DX_DEBUG_POSTFIX}_43.dll
    d3dx11${D3DX_DEBUG_POSTFIX}_43.dll
)
if(CMAKE_VS_PLATFORM_NAME MATCHES "^(x86|Win32)$")
    set(D3DX_PLATFORM "x86")
elseif(CMAKE_VS_PLATFORM_NAME MATCHES "^x64$")
    set(D3DX_PLATFORM "x64")
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86)$")
        set(D3DX_PLATFORM "x86")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|x64)$")
        set(D3DX_PLATFORM "x64")
    endif()
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_DXSDK_D3DX)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/${D3DX_CONFIGURATION}/lib/${D3DX_PLATFORM})
target_link_libraries(${PROJECT_NAME} PRIVATE d3dx9${D3DX_DEBUG_POSTFIX}.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE d3dx10${D3DX_DEBUG_POSTFIX}.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE d3dx11${D3DX_DEBUG_POSTFIX}.lib)
foreach(D3DX_BINARY ${D3DX_BINARIES})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/${D3DX_CONFIGURATION}/bin/${D3DX_PLATFORM}/${D3DX_BINARY}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        COMMENT "Copying ${D3DX_BINARY}..."
    )
endforeach()

# Microsoft DirectXMath
target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_DIRECTXMATH)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/directxmath.2024.10.15.1/include)

# Microsoft DirectXMesh
set(DIRECTXMESH_CONFIGURATION "$<IF:$<CONFIG:DEBUG>,Debug,Release>")
if(CMAKE_VS_PLATFORM_NAME MATCHES "^(x86|Win32)$")
    set(DIRECTXMESH_PLATFORM "x86")
elseif(CMAKE_VS_PLATFORM_NAME MATCHES "^x64$")
    set(DIRECTXMESH_PLATFORM "x64")
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86)$")
        set(DIRECTXMESH_PLATFORM "x86")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|x64)$")
        set(DIRECTXMESH_PLATFORM "x64")
    endif()
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_DIRECTXMESH)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/directxmesh_desktop_win10.2025.7.10.1/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/directxmesh_desktop_win10.2025.7.10.1/native/lib/${DIRECTXMESH_PLATFORM}/${DIRECTXMESH_CONFIGURATION})
target_link_libraries(${PROJECT_NAME} PRIVATE DirectXMesh.lib)

# Microsoft DirectXTex
set(DIRECTXTEX_CONFIGURATION "$<IF:$<CONFIG:DEBUG>,Debug,Release>")
if(CMAKE_VS_PLATFORM_NAME MATCHES "^(x86|Win32)$")
    set(DIRECTXTEX_PLATFORM "x86")
elseif(CMAKE_VS_PLATFORM_NAME MATCHES "^x64$")
    set(DIRECTXTEX_PLATFORM "x64")
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86)$")
        set(DIRECTXTEX_PLATFORM "x86")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|x64)$")
        set(DIRECTXTEX_PLATFORM "x64")
    endif()
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_DIRECTXTEX)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/directxtex_desktop_win10.2025.7.10.1/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/directxtex_desktop_win10.2025.7.10.1/native/lib/${DIRECTXTEX_PLATFORM}/${DIRECTXTEX_CONFIGURATION})
target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTex.lib)

# GLFW
if(CMAKE_VS_PLATFORM_NAME MATCHES "^(x86|Win32)$")
    set(GLFW_PLATFORM "win32")
elseif(CMAKE_VS_PLATFORM_NAME MATCHES "^x64$")
    set(GLFW_PLATFORM "x64")
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86)$")
        set(GLFW_PLATFORM "win32")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|x64)$")
        set(GLFW_PLATFORM "x64")
    endif()
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE HAS_GLFW)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/glfw.3.4.0/build/native/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/glfw.3.4.0/build/native/lib/dynamic/v143/${GLFW_PLATFORM})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw3dll.lib)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/glfw.3.4.0/build/native/bin/dynamic/v143/${GLFW_PLATFORM}/glfw3.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
    COMMENT "Copying glfw3.dll..."
)
