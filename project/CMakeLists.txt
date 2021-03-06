cmake_minimum_required(VERSION 3.15)
project(open_carom3d_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(../src)
include_directories(../src/core/lib)

add_executable(open_carom3d_server
        ../src/business/game_server/recv_actions.h
        ../src/business/game_server/send_actions.h
        ../src/business/game_server/game_server.cpp
        ../src/business/game_server/game_server.h
        ../src/business/game_server/game_server_action.h
        ../src/business/management_server/actions/client_version_action.cpp
        ../src/business/management_server/actions/client_version_action.h
        ../src/business/management_server/actions/login_action.cpp
        ../src/business/management_server/actions/login_action.h
        ../src/business/management_server/actions/server_list_request_action.cpp
        ../src/business/management_server/actions/server_list_request_action.h
        ../src/business/management_server/management_server.cpp
        ../src/business/management_server/management_server.h
        ../src/business/entity/account.cpp
        ../src/business/entity/account.h
        ../src/business/entity/channel.cpp
        ../src/business/entity/channel.h
        ../src/business/entity/match.cpp
        ../src/business/entity/match.h
        ../src/business/entity/player.cpp
        ../src/business/entity/player.h
        ../src/business/entity/room.cpp
        ../src/business/entity/room.data.cpp
        ../src/business/entity/room.h
        ../src/business/entity/user.cpp
        ../src/business/entity/user.h
        ../src/business/entity/user_spot.h
        ../src/business/service/account_service.cpp
        ../src/business/service/account_service.h
        ../src/business/service/channel_service.cpp
        ../src/business/service/channel_service.h
        ../src/business/service/room_service.cpp
        ../src/business/service/room_service.h
        ../src/business/service/server_service.cpp
        ../src/business/service/server_service.h
        ../src/business/service/user_service.cpp
        ../src/business/service/user_service.h
        ../src/business/util/action_dispatcher.cpp
        ../src/business/util/action_dispatcher.h
        ../src/business/util/destination.cpp
        ../src/business/util/destination.h
        ../src/core/lib/nettools/ntClient.cpp
        ../src/core/lib/nettools/ntClient.h
        ../src/core/lib/nettools/ntConnection.cpp
        ../src/core/lib/nettools/ntConnection.h
        ../src/core/lib/nettools/ntServer.cpp
        ../src/core/lib/nettools/ntServer.h
        ../src/core/lib/nettools/ntUtils.cpp
        ../src/core/lib/nettools/ntUtils.h
        ../src/core/server/carom3d/action.h
        ../src/core/server/carom3d/carom3d_protocol.cpp
        ../src/core/server/carom3d/carom3d_protocol.h
        ../src/core/server/carom3d/carom3d_server.cpp
        ../src/core/server/carom3d/carom3d_server.h
        ../src/core/server/carom3d/carom3d_user_session.h
        ../src/core/server/carom3d/crypto.cpp
        ../src/core/server/carom3d/crypto.h
        ../src/core/server/carom3d/carom3d_message_parser.cpp
        ../src/core/server/carom3d/carom3d_message_parser.h
        ../src/core/server/carom3d/util/action_builder.h
        ../src/core/server/http/http_server.cpp
        ../src/core/server/http/http_server.h
        ../src/core/server/client_session.cpp
        ../src/core/server/client_session.h
        ../src/core/server/messaging_protocol.h
        ../src/core/server/server_config.h
        ../src/core/server/server_context.cpp
        ../src/core/server/server_context.h
        ../src/core/util/types.h
        ../src/server_starter.cpp)

add_custom_command(TARGET open_carom3d_server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/../resources $<TARGET_FILE_DIR:open_carom3d_server>/resources)

#file(COPY ${PROJECT_SOURCE_DIR}/../resources DESTINATION ${CMAKE_BINARY_DIR})