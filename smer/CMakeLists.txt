
# add_subdirectory(app)

# set(TT_CORE_LIB te)
# project(TT_CORE_LIB)

include(Actor/CMakeLists.txt)
include(Client/CMakeLists.txt)
include(Command/CMakeLists.txt)
include(Communication/CMakeLists.txt)
include(Game/CMakeLists.txt)
include(HandleMsg/CMakeLists.txt)
include(Input/CMakeLists.txt)
include(IOMange/CMakeLists.txt)
include(Mysql/CMakeLists.txt)
include(Player/CMakeLists.txt)
include(Redis/CMakeLists.txt)
include(Seqeue/CMakeLists.txt)
include(Server/CMakeLists.txt)
include(SmerSystem/CMakeLists.txt)
include(Util/CMakeLists.txt)

set(ALL_LIB_SOURCE)
set(ALL_LIB_HEADER)

set(SMER_HEADER
${SMER_ACTOR_HEADER}
${SMER_GAME_HEADER}
${SMER_CLIENT_HEADER}
${SMER_COMMAND_HEADER}
${SMER_HANDLEMSG_HEADER}
${SMER_COMMUNICATION_HEADER}
${SMER_IOMANGE_HEADER}
${SMER_MYSQL_HEADER}
${SMER_PLAYER_HEADER}
${SMER_INPUT_HEADER}
${SMER_REDIS_HEADER}
${SMER_SEQEUE_HEADER}
${SMER_SMERSYSTEM_HEADER}
${SMER_UTIL_HEADER}
${SMER_SERVER_HEADER}
)

set(SMER_SRC
${SMER_ACTOR_SRC}
${SMER_GAME_SRC}
${SMER_CLIENT_SRC}
${SMER_COMMAND_SRC}
${SMER_HANDLEMSG_SRC}
${SMER_COMMUNICATION_SRC}
${SMER_IOMANGE_SRC}
${SMER_MYSQL_SRC}
${SMER_PLAYER_SRC}
${SMER_INPUT_SRC}
${SMER_REDIS_SRC}
${SMER_SEQEUE_SRC}
${SMER_SMERSYSTEM_SRC}
${SMER_UTIL_SRC}
${SMER_SERVER_SRC}
)


list(APPEND ALL_LIB_HEADER
$(SMER_HEADER)
)

set(APP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/smer/)

list(APPEND SMER_SRC ${SMER_HEADER})

add_library(smer ${SMER_SRC})

target_include_directories(smer
    PUBLIC ${APP_SOURCE_DIR}/Actor/inc
    PUBLIC ${APP_SOURCE_DIR}/Client/inc
    PUBLIC ${APP_SOURCE_DIR}/Command/inc
    PUBLIC ${APP_SOURCE_DIR}/Communication/inc
    PUBLIC ${APP_SOURCE_DIR}/Game/inc
    PUBLIC ${APP_SOURCE_DIR}/HandleMsg/inc
    PUBLIC ${APP_SOURCE_DIR}/Input/inc
    PUBLIC ${APP_SOURCE_DIR}/IOMange/inc
    PUBLIC ${APP_SOURCE_DIR}/Mysql/inc
    PUBLIC ${APP_SOURCE_DIR}/Player/inc
    PUBLIC ${APP_SOURCE_DIR}/Redis/inc
    PUBLIC ${APP_SOURCE_DIR}/Seqeue/inc
    PUBLIC ${APP_SOURCE_DIR}/Server/inc
    PUBLIC ${APP_SOURCE_DIR}/SmerSystem/inc
    PUBLIC ${APP_SOURCE_DIR}/Util/inc
    PUBLIC /usr/local/Cellar/boost/1.70.0/include
)

Message(${PROJECT_SOURCE_DIR})