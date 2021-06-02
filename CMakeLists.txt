cmake_minimum_required(VERSION 3.0.0)

option(TESTING "enable test compilation" OFF)
option(SERVER_COMPARE "enable server compilation" OFF)
option(CLIENT_COMPARE "enable client compilation" OFF)

if(TESTING)
    set(APP_NAME test)
else()
    set(APP_NAME main)
endif()

add_compile_options(-lpthread)

if(CLIENT_COMPARE)
    add_compile_options(-D CLIENT_COMPARE)
else()
    add_compile_options(-D SERVER_COMPARE)
endif()

add_compile_options(-std=c++11 -g)

project(${APP_NAME})

add_subdirectory(${PROJECT_SOURCE_DIR}/smer/)

set(TEST_SOURCE)
set(TEST_HEADER)

if(TESTING)
    list(APPEND TEST_SOURCE
    ${PROJECT_SOURCE_DIR}/tests/test.cpp
    )
else()
    list(APPEND TEST_SOURCE
    ${PROJECT_SOURCE_DIR}/client/client.cpp
    )
endif()

set(all_code_files
    ${TEST_SOURCE}
    ${TEST_HEADER}
)

add_executable(${APP_NAME} ${all_code_files})
target_link_libraries(${APP_NAME} smer)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)