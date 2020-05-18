//
// Created by CGR on 16/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACCOUNT_SERVICE_H__
#define __OPEN_CAROM3D_SERVER_ACCOUNT_SERVICE_H__

#include <list>
#include <map>
#include <core/server/client_session.h>

class Account;
class Player;

class AccountService {
    std::list<Account*> m_accounts;
    std::list<Player*> m_players;
    std::map<int, Account*> m_clientsAccounts;

    Account *findAccount(const wchar_t *accountName);

public:
    static AccountService& getInstance();
    Account *createAccount(const wchar_t *accountName, const wchar_t *accountPassword);
    Player *createPlayerFromAccount(Account &account);

    Account* logAccount(ClientSession &client, const wchar_t *accountName, const wchar_t *accountPassword);
};

#endif //__OPEN_CAROM3D_SERVER_ACCOUNT_SERVICE_H__
