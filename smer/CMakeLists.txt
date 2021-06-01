
# add_subdirectory(app)

# set(TT_CORE_LIB te)
# project(TT_CORE_LIB)

include(fun/CMakeLists.txt)
include(render/CMakeLists.txt)

set(ALL_LIB_SOURCE)
set(ALL_LIB_HEADER)

set(TE_HEADER
${TE_FUN_HEADER}
${TE_RENDER_HEADER}
)

set(TE_SRC
${TE_FUN_SRC}
${TE_RENDER_SRC}
)


list(APPEND ALL_LIB_HEADER
$(TE_HEADER)
)

set(APP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/smer/)


add_library(smer ${TE_SRC})

target_include_directories(smer
    PUBLIC ${APP_SOURCE_DIR}/fun/inc
    PUBLIC ${APP_SOURCE_DIR}/render/inc
    PUBLIC /usr/local/Cellar/boost/1.70.0/include
)

Message(${PROJECT_SOURCE_DIR})