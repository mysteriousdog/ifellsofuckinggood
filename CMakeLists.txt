cmake_minimum_required (VERSION 3.0.2)

option(TESTING "enable test compilation" OFF)
option(SERVER_COMPARE "enable server compilation" OFF)
option(CLIENT_COMPARE "enable client compilation" OFF)

if(TESTING)
    set(APP_NAME test)
endif()

if(SERVER_COMPARE)
    set(APP_NAME server)
endif()

if(CLIENT_COMPARE)
    set(APP_NAME client)
endif()

add_compile_options(-std=c++11 -g)

if(CLIENT_COMPARE)
    add_compile_options(-D CLIENT_COMPARE)
    add_definitions(-D BOOST_LOG_DYN_LINK)
else()
    add_compile_options(-D SERVER_COMPARE)
    add_definitions(-D _GLIBCXX_USE_CXX11_ABI=0)
endif()

project(${APP_NAME})

add_subdirectory(${PROJECT_SOURCE_DIR}/smer/)

set(ALL_RUN_SOURCE)
set(ALL_RUN_HEADER)

if(SERVER_COMPARE)
    list(APPEND ALL_RUN_SOURCE
    ${PROJECT_SOURCE_DIR}/tests/serverTest.cpp
    )
endif()

if(CLIENT_COMPARE)
    list(APPEND ALL_RUN_SOURCE
    ${PROJECT_SOURCE_DIR}/tests/clientTest.cpp
    )
endif()

set(all_code_files
    ${ALL_RUN_SOURCE}
    ${ALL_RUN_HEADER}
)

LINK_DIRECTORIES(/usr/local/lib)



add_executable(${APP_NAME} ${all_code_files})
target_link_libraries(${APP_NAME} smer)

if(SERVER_COMPARE)
    target_link_libraries(${APP_NAME} libmysqlcppconn.so.7.1.1.8)
    target_link_libraries(${APP_NAME} librediscluster.so)
    target_link_libraries(${APP_NAME} libboost_filesystem.so)
    target_link_libraries(${APP_NAME} libboost_system.so)
    target_link_libraries(${APP_NAME} libboost_date_time.so)
    find_package(Threads)
    target_link_libraries(${APP_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

if(CLIENT_COMPARE)
    target_link_libraries(${APP_NAME} libboost_filesystem-mt.dylib)
    target_link_libraries(${APP_NAME} libboost_system-mt.dylib)
    target_link_libraries(${APP_NAME} libboost_log-mt.dylib)
    target_link_libraries(${APP_NAME} libboost_log_setup-mt.dylib)
    target_link_libraries(${APP_NAME} libboost_thread-mt.dylib)
    target_link_libraries(${APP_NAME} libboost_date_time-mt.dylib)
    target_link_libraries(${APP_NAME} libboost_serialization-mt.dylib)
endif()


set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)