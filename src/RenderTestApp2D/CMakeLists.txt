# Project for DirectX 2D render test application
project(RenderTestApp2D
    LANGUAGES CXX
    VERSION ${SPG_VERSION}
)

set(SPG_RENDER_DX2DA_BUILD_DEFINES
    _WINDOWS
)
set(SPG_RENDER_DX2DA_BUILD_FLAGS
    ${SPG_CXX_FLAGS}
)
set(SPG_RENDER_DX2DA_LINK_FLAGS
    ${SPG_LINKER_FLAGS}
    /SUBSYSTEM:WINDOWS
)

set(SPG_RENDER_DX2DA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTestApp2D.cpp
)
set(SPG_RENDER_DX2DA_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framework.h
    ${CMAKE_CURRENT_SOURCE_DIR}/targetver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTestApp2D.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource.h
)
set(SPG_RENDER_DX2DA_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTestApp2D.rc
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTestApp2D.ico
    ${CMAKE_CURRENT_SOURCE_DIR}/small.ico
)

add_executable(${PROJECT_NAME}
    ${SPG_RENDER_DX2DA_SOURCES}
    ${SPG_RENDER_DX2DA_INCLUDES}
    ${SPG_RENDER_DX2DA_RESOURCES}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.hpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${SPG_RENDER_DX2DA_BUILD_DEFINES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_DX2DA_BUILD_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_DX2DA_LINK_FLAGS})

target_link_libraries(${PROJECT_NAME} PRIVATE d3d11.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE winmm.lib)

add_dependencies(${PROJECT_NAME} RenderDirectX2D)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../RenderDirectX2D)
target_link_libraries(${PROJECT_NAME} PRIVATE RenderDirectX2D)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
    COMMENT "Copying data..."
)
