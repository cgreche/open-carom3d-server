//
// Created by CGR on 14/05/2020.
//

#include <map>
#include "../core/server/action.h"

#include "actions/login_action.h"
#include "actions/create_Account_action.h"
#include "actions/send_message_action.h"
#include "actions/send_private_message_action.h"

#define LOGIN_ACTION 0x01
#define CREATE_ACCOUNT_ACTION 0x02

std::map<int, Action*> actions = {
        { LOGIN_ACTION, new LoginAction },
        { CREATE_ACCOUNT_ACTION, new CreateAccountAction }
};

