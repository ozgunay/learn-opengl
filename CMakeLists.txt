cmake_minimum_required(VERSION 3.6)
project(learn-opengl VERSION 0.0.1 DESCRIPTION "Learn OpenGL with Magnum Engine" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/modules")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/modules")

set( LIB_DIR     lib )
set( SRC_DIR     src )

file( GLOB_RECURSE SOURCES
        ${SRC_DIR}/*.h
        ${SRC_DIR}/*.hpp
        ${SRC_DIR}/*.tpp
        ${SRC_DIR}/*.cpp )

foreach(_source IN ITEMS ${SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

#       Compiler-specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_link_options(-pthread -lpthread)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -O3 -Wa,-mbig-obj" )
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -O3 -Wa,-mbig-obj" )
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /W4" )

    # required by corrade
    set(MSVC2019_COMPATIBILITY TRUE)
endif()

#       GLWF
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# These two will be off-by-default when GLFW 3.4 gets released
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#       magnum
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(WITH_GLFWAPPLICATION   ON CACHE BOOL "" FORCE)

add_subdirectory(${LIB_DIR}/glfw               EXCLUDE_FROM_ALL)
add_subdirectory(${LIB_DIR}/corrade            EXCLUDE_FROM_ALL)
add_subdirectory(${LIB_DIR}/magnum             EXCLUDE_FROM_ALL)

add_executable(learn-opengl
    ${SOURCES})

if(CORRADE_TARGET_ANDROID)
    find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED GlfwApplication)
endif()

find_package(Magnum REQUIRED
        GL
        Trade
        Shaders
        MeshTools
        SceneGraph
        DebugTools
        Primitives)
     
target_link_libraries(learn-opengl PUBLIC
        Corrade::Main
        Magnum::GL
        Magnum::Trade
        Magnum::Magnum
        Magnum::Shaders
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::SceneGraph
        Magnum::DebugTools
        Magnum::Application)

install(TARGETS learn-opengl DESTINATION ${MAGNUM_BINARY_INSTALL_DIR})
# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT learn-opengl)