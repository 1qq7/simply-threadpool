cmake_minimum_required(VERSION 2.8)
project(threadpool)

include_directories(
    ${CMAKE_SOURCE_DIR}/../threadpool/include
)

set(SOURCE_FILES
    main.cc
)

set(target_name ${PROJECT_NAME}_sample)
add_executable(${target_name} ${SOURCE_FILES})
target_link_libraries(${target_name} pthread)