cmake_minimum_required(VERSION 3.16)
project(open_carom3d_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(../src)
include_directories(../src/core/lib)

add_executable(open_carom3d_server
        ../src/business/actions/create_account_action.cpp
        ../src/business/actions/create_account_action.h
        ../src/business/actions/login_action.cpp
        ../src/business/actions/login_action.h
        ../src/business/actions/send_message_action.cpp
        ../src/business/actions/send_message_action.h
        ../src/business/actions/send_private_message_action.cpp
        ../src/business/actions/send_private_message_action.h
        ../src/business/action_mapper.cpp
        ../src/core/lib/nettools/ntClient.cpp
        ../src/core/lib/nettools/ntClient.h
        ../src/core/lib/nettools/ntConnection.cpp
        ../src/core/lib/nettools/ntConnection.h
        ../src/core/lib/nettools/ntServer.cpp
        ../src/core/lib/nettools/ntServer.h
        ../src/core/lib/nettools/ntUtils.cpp
        ../src/core/lib/nettools/ntUtils.h
        ../src/core/server/client_session.cpp
        ../src/core/server/client_session.h
        ../src/core/server/server_config.h
        ../src/core/server/server_context.cpp
        ../src/core/server/server_context.h
        ../src/core/server/action.h
        ../src/core/server/messaging/crypto.cpp
        ../src/core/server/messaging/crypto.h
        ../src/core/server/messaging/message_listener.cpp
        ../src/core/server/messaging/message_listener.h
        ../src/core/server/messaging/message_parser.cpp
        ../src/core/server/messaging/message_parser.h
        ../src/server_starter.cpp)
