cmake_minimum_required(VERSION 3.4)

project(MagnumExamples CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/modules")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/modules")

set( LIB_DIR     lib )

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

add_subdirectory(${LIB_DIR}/glfw               EXCLUDE_FROM_ALL)
add_subdirectory(${LIB_DIR}/corrade            EXCLUDE_FROM_ALL)
add_subdirectory(${LIB_DIR}/magnum             EXCLUDE_FROM_ALL)

#       GLWF
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# These two will be off-by-default when GLFW 3.4 gets released
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


include(CMakeDependentOption)

option(WITH_BASE_PROJECT "Build base project" ON)
option(WITH_TRIANGLE_EXAMPLE "Build triangle example" ON)

add_subdirectory(src)