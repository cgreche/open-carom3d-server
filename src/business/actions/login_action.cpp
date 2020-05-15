//
// Created by CGR on 14/05/2020.
//

#include <core/server/client_session.h>
#include "login_action.h"

bool LoginAction::validate(ActionData &action) {
    int size = action.data().size();
    int _size = sizeof(LoginData);
    return size == _size;
}

void LoginAction::execute(ActionData &action, ClientSession &client, const LoginData *data) {
    printf("Player logged in: %ws, %ws", data->username, data->password);
//    const unsigned char loginResult[0x8E] = {0};
//    *(int*)&loginResult[0x5A] = 0x00;
//    ActionData actionData(0x00, (unsigned char*)&loginResult, sizeof(loginResult));
//    client.sendAction(actionData);
}
