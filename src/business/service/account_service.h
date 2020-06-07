//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACCOUNT_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_ACCOUNT_SERVICE_H__

#include <list>
#include <map>
#include <core/server/client_session.h>
#include "business/entity/room.h"

namespace business {

    class Account;
    class Player;

    class AccountService {

    public:
        static AccountService &getInstance();

        Account *createAccount(const wchar_t *accountName, const wchar_t *accountPassword);
        Player *createPlayerFromAccount(Account &account);
        Account *logAccount(const wchar_t *accountName, const wchar_t *accountPassword);
        Account* findAccount(const wchar_t* accountName);

        Player* getPlayer(Account& account);

    };

}

#endif //__OPEN_CAROM3D_SERVER_ACCOUNT_SERVICE_H__
