cmake_minimum_required(VERSION 3.6)

project(base-project CXX)
set(CMAKE_CXX_STANDARD 17)

# Add module path in case this is project root
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../lib/modules/" ${CMAKE_MODULE_PATH})
endif()

#       magnum
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(WITH_GLFWAPPLICATION   ON CACHE BOOL "" FORCE)

find_package(Corrade REQUIRED Main)

if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED GlfwApplication)
endif()

find_package(Magnum REQUIRED
    GL
    MeshTools
    Primitives
    Shaders
    Trade)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(base-project WIN32 main.cpp)
target_link_libraries(base-project PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::Trade)

install(TARGETS base-project DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT base-project)