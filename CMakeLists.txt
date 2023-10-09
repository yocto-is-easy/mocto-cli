project(mocto-cli)

cmake_minimum_required(VERSION 3.12)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(
    include/
)

add_executable(mocto-cli
    src/main.cpp
    src/SubCmd.cpp
)

target_include_directories(mocto-cli PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(mocto-cli ${Boost_LIBRARIES})
