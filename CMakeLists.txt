cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME RenderPipeline)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)


function(verify_path NAME PATH)
    message(CHECK_START "Verifying ${NAME} path (${PATH})")
    if(IS_DIRECTORY ${PATH})
        message(CHECK_PASS "Found")
    else()
        message(CHECK_FAIL "Not Found")
        message(FATAL_ERROR "Path (${PATH}) does not exist.")
    endif()
endfunction()


string(TOLOWER ${CMAKE_CXX_COMPILER_ID} COMP)
string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM)

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL AMD64)
    set(ARCH "x64")
else()
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
endif ()

set(COMPILER_TRIPLET "${COMP}-${PLATFORM}-${ARCH}")
message(STATUS "Compiling for: ${COMPILER_TRIPLET}")

set(VENDOR_INCLUDE_DIR  ${CMAKE_SOURCE_DIR}/vendor/include)
set(VENDOR_LIB_DIR      ${CMAKE_SOURCE_DIR}/vendor/${COMPILER_TRIPLET}/lib)
set(VENDOR_SRC_DIR      ${CMAKE_SOURCE_DIR}/vendor/src)

verify_path("Vendor Include"    ${VENDOR_INCLUDE_DIR})
verify_path("Vendor Lib"        ${VENDOR_LIB_DIR})
verify_path("Vendor Source"     ${VENDOR_SRC_DIR})


add_executable(${PROJECT_NAME}
        src/Main.cpp

        src/core/Core.cpp                       src/core/Core.h
        src/core/GlDebugger.cpp                 src/core/GlDebugger.h
        src/core/Scene.cpp                      include/core/Scene.h
        src/core/CameraSystem.cpp               include/core/CameraSystem.h
        src/core/CameraControllerSystem.cpp     include/core/CameraControllerSystem.h
        include/core/Components.h

        src/ecs/EntityManager.cpp src/ecs/EntityManager.h
        src/ecs/ComponentArray.h
        src/ecs/ComponentManager.h
        src/ecs/SystemManager.h
        include/ecs/System.h
        include/ecs/EcsCommon.h
        include/ecs/EcsDirector.h

        src/renderer/RendererSystem.cpp         include/renderer/RendererSystem.h
        src/renderer/Primitives.cpp             include/renderer/Primitives.h
        src/renderer/Vertex.cpp                 include/renderer/Vertex.h
        src/renderer/Shader.cpp                 include/renderer/Shader.h
        src/renderer/TextureSystem.cpp          include/renderer/TextureSystem.h
        src/renderer/MaterialProcessor.cpp      include/renderer/MaterialProcessor.h
        src/renderer/PointLightTransformer.cpp  include/renderer/PointLightTransformer.h

        src/loader/ObjLoader.cpp        src/loader/ObjLoader.h
        src/loader/LoaderCommon.cpp     src/loader/LoaderCommon.h
        src/loader/MtlLoader.cpp        src/loader/MtlLoader.h
        src/loader/Loader.cpp           include/loader/Loader.h

        ${VENDOR_SRC_DIR}/imgui/imgui.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_demo.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_draw.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_tables.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_widgets.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_impl_glfw.cpp
        ${VENDOR_SRC_DIR}/imgui/imgui_impl_opengl3.cpp
        )


target_precompile_headers(${PROJECT_NAME} PUBLIC
        <iostream> <vector> <unordered_map> <string> <string_view> <algorithm> <memory> <numeric>   # STL
        <glm.hpp> <gtx/quaternion.hpp> <glew.h>                                                     # Vendor
        )

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

        ${VENDOR_INCLUDE_DIR}
        ${VENDOR_INCLUDE_DIR}/GL
        ${VENDOR_INCLUDE_DIR}/GLFW
        ${VENDOR_INCLUDE_DIR}/glm
        ${VENDOR_INCLUDE_DIR}/imgui
        ${VENDOR_INCLUDE_DIR}/stb-image
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        GLEW_STATIC
        STB_IMAGE_IMPLEMENTATION
)

find_package(OpenGL REQUIRED)
find_library(GLEW NAMES glew32s PATHS ${VENDOR_LIB_DIR} REQUIRED)
find_library(GLFW NAMES glfw3 PATHS ${VENDOR_LIB_DIR} REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL ${GLEW} -NODEFAULTLIB:glew32s ${GLFW})
