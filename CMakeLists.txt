cmake_minimum_required(VERSION 2.8.12)
project(ht)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

include_directories(include)
include_directories(${Boost_INCLUDE_DIR})

file(GLOB SOURCES_SERVER "src/server/*.cpp")

file(GLOB SOURCES_CLIENT "src/client/*.cpp")

add_executable(ht_server ${SOURCES_SERVER})
add_executable(ht_client ${SOURCES_CLIENT})

target_link_libraries(ht_server Boost::program_options)
target_link_libraries(ht_client Boost::program_options)
