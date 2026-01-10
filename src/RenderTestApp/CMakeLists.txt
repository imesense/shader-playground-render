# Project for render test application
project(RenderTestApp
    LANGUAGES CXX
    VERSION ${SPG_VERSION}
)

set(SPG_RENDER_TESTAPP_BUILD_DEFINES
    _WINDOWS
    _UNICODE
)
set(SPG_RENDER_TESTAPP_BUILD_FLAGS
    ${SPG_CXX_FLAGS}
)
set(SPG_RENDER_TESTAPP_LINK_FLAGS
    ${SPG_LINKER_FLAGS}
    /SUBSYSTEM:WINDOWS
)

set(SPG_RENDER_TESTAPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Program.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StartUp.cpp
)
set(SPG_RENDER_TESTAPP_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/framework.h
    ${CMAKE_CURRENT_SOURCE_DIR}/StartUp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Program.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TargetVersion.h
)
set(SPG_RENDER_TESTAPP_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resource.rc
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderTestApp.ico
    ${CMAKE_CURRENT_SOURCE_DIR}/small.ico
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../RenderLibrary)

add_executable(${PROJECT_NAME}
    ${SPG_RENDER_TESTAPP_SOURCES}
    ${SPG_RENDER_TESTAPP_INCLUDES}
    ${SPG_RENDER_TESTAPP_RESOURCES}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${SPG_RENDER_TESTAPP_BUILD_DEFINES})
target_compile_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_TESTAPP_BUILD_FLAGS})
target_link_options(${PROJECT_NAME} PRIVATE ${SPG_RENDER_TESTAPP_LINK_FLAGS})

add_dependencies(${PROJECT_NAME} RenderLibrary)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../RenderLibrary)
target_link_libraries(${PROJECT_NAME} PRIVATE RenderLibrary)

add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${NUGET_COMMAND} restore
        "${CMAKE_CURRENT_SOURCE_DIR}/packages.config"
        -SolutionDirectory "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Restoring packages..."
)

# Nlohmann JSON
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../pkg/nlohmann.json.3.12.0/build/native/include)
