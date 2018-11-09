cmake_minimum_required(VERSION 3.6)
project(motan_lua)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -std=c99 -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNODEBUG -std=c99 -Wall")

# for gcc we need this macro for ifreq
add_definitions(-D_BSD_SOURCE)

include_directories(./)
# we should use case sensitive package name for linux
find_package(Lua REQUIRED)
set(SOURCE_FILES serialization.c utils.c buffer.c)

add_library(motan_shared SHARED ${SOURCE_FILES})
SET_TARGET_PROPERTIES(motan_shared PROPERTIES OUTPUT_NAME "motan")
target_link_libraries(motan_shared Lua)

add_library(motan_static STATIC ${SOURCE_FILES})
SET_TARGET_PROPERTIES(motan_static PROPERTIES OUTPUT_NAME "motan")
target_link_libraries(motan_static Lua)

add_executable(buffer_unittest buffer_unittest.c)
target_link_libraries(buffer_unittest motan_static)