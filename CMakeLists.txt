cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME RenderPipeline)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        src/Main.cpp
        src/core/Core.cpp src/core/Core.h
        src/ecs/EntityManager.h src/ecs/EntityManager.cpp
        src/ecs/ComponentArray.h
        src/ecs/ComponentManager.h
        include/ecs/System.h
        src/ecs/SystemManager.h

        include/core/Scene.h
        include/ecs/EcsCommon.h
        include/ecs/EcsDirector.h

        include/core/Components.h src/core/Scene.cpp
        src/renderer/RendererSystem.cpp include/renderer/RendererSystem.h
        include/renderer/Vertex.h src/renderer/Primitives.cpp
        include/renderer/Primitives.h src/renderer/Vertex.cpp
        include/renderer/Shader.h src/renderer/Shader.cpp
        src/core/GlDebugger.cpp src/core/GlDebugger.h
        src/core/CameraSystem.cpp include/core/CameraSystem.h
        src/core/CameraControllerSystem.cpp include/core/CameraControllerSystem.h
        src/renderer/TextureSystem.cpp include/renderer/TextureSystem.h
        src/loader/Loader.cpp include/loader/Loader.h
        src/loader/ObjLoader.cpp src/loader/ObjLoader.h src/loader/LoaderCommon.cpp src/loader/LoaderCommon.h src/loader/MtlLoader.cpp src/loader/MtlLoader.h src/renderer/MaterialProcessor.cpp include/renderer/MaterialProcessor.h src/renderer/PointLightTransformer.cpp include/renderer/PointLightTransformer.h)

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        include/ecs
        include/core
        include/renderer
        include/common
        include/loader

        src/core
        src/ecs
        src/common
        src/loader

        sandbox
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        GLEW_STATIC
        STB_IMAGE_IMPLEMENTATION
)


# Vendor Includes and libs
string(TOLOWER ${CMAKE_CXX_COMPILER_ID} COMP)
string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL AMD64)
    set(ARCH "x64")
else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
endif ()

set(COMPILER_TRIPLET "${COMP}-${PLATFORM}-${ARCH}")
message(STATUS "Compiling For: ${COMPILER_TRIPLET}")

# Safety Check to see if the include folder exists.
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/include)
message(CHECK_START "Looking for Vendor Includes at: ${INCLUDE_DIR}")
if (IS_DIRECTORY ${INCLUDE_DIR})
    message(CHECK_PASS "Found")
else()
    message(CHECK_FAIL "Failed")
    message(FATAL_ERROR "Vendor include does not exists for type: ${COMPILER_TRIPLET}")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
        ${INCLUDE_DIR}
        ${INCLUDE_DIR}/GL
        ${INCLUDE_DIR}/GLFW
        ${INCLUDE_DIR}/glm
        ${INCLUDE_DIR}/imgui
        ${INCLUDE_DIR}/stb-image
        )

# Setting up vendor libs directory
set(VENDOR_LIB ${CMAKE_SOURCE_DIR}/vendor/${COMPILER_TRIPLET}/lib)
message(CHECK_START "Looking for Vendor Lib at: ${VENDOR_LIB}")
if (IS_DIRECTORY ${VENDOR_LIB})
    message(CHECK_PASS "Found")
else()
    message(CHECK_FAIL "Failed")
    message(FATAL_ERROR "Vendor lib does not exist for type: ${COMPILER_TRIPLET}.\n Check path exist: ${VENDOR_LIB}")
endif ()

find_package(OpenGL REQUIRED)
find_library(GLEW NAMES glew32 glew glew32s PATHS ${VENDOR_LIB} REQUIRED)
find_library(GLFW glfw3 ${VENDOR_LIB} REQUIRED)
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        ${GLEW}
        ${GLFW}
        )


# Safety Check to see if the src folder exists.
set(VENDOR_SRC_DIR ${CMAKE_SOURCE_DIR}/vendor/src)
message(CHECK_START "Looking for Vendor src at: ${VENDOR_SRC_DIR}")
if (IS_DIRECTORY ${VENDOR_SRC_DIR})
    message(CHECK_PASS "Found")
else()
    message(CHECK_FAIL "Failed")
    message(FATAL_ERROR "Vendor src does not exists for type: ${COMPILER_TRIPLET}")
endif()

# Making imgui a static library
add_library(imgui STATIC
        ${VENDOR_SRC_DIR}/imgui/imgui.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_demo.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_draw.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_tables.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_widgets.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_impl_glfw.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_impl_opengl3.cpp
        )

target_include_directories(imgui PUBLIC
        ${INCLUDE_DIR}
        ${INCLUDE_DIR}/imgui
        )

target_link_libraries(${PROJECT_NAME} imgui)