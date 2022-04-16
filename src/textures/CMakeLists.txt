cmake_minimum_required(VERSION 3.6)

project(textures CXX)
set(CMAKE_CXX_STANDARD 17)

# Add module path in case this is project root
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../lib/modules/" ${CMAKE_MODULE_PATH})
endif()

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

find_package(MagnumPlugins REQUIRED StbImageImporter)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
corrade_add_resource(textures_RESOURCES resources.conf)

add_executable(textures 
    main.cpp
    MyShader.h
    MyShader.cpp
    ${textures_RESOURCES})

add_dependencies(textures
    Magnum::AnyImageImporter
    MagnumPlugins::StbImageImporter)

target_link_libraries(textures PRIVATE
    Corrade::Main
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Shaders
    Magnum::Trade)

install(TARGETS textures DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT textures)