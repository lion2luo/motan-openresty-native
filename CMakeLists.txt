cmake_minimum_required(VERSION 3.6)
project(motan_lua)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNODEBUG -std=c99 -Wall -Wextra")

# for gcc we need this macro for ifreq
add_definitions(-D_BSD_SOURCE)

# we use luajit 2.0 to compile, the generated library still can running with luajit-2.1
include_directories(src /usr/local/include/luajit-2.0)
# we should use case sensitive package name for linux
set(SOURCE_FILES src/serialization.c src/buffer.c)

add_library(motan_shared SHARED ${SOURCE_FILES})
SET_TARGET_PROPERTIES(motan_shared PROPERTIES OUTPUT_NAME "motan")
target_link_libraries(motan_shared luajit-5.1)

add_executable(buffer_unittest src/buffer_unittest.c)
target_link_libraries(buffer_unittest motan_shared)

add_executable(serialization_unittest src/serialization_unittest.c)
target_link_libraries(serialization_unittest luajit-5.1)