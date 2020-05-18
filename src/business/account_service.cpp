//
// Created by CGR on 16/05/2020.
//


#include <core/server/client_session.h>
#include "account_service.h"
#include "player.h"

AccountService g_accountService;

AccountService& AccountService::getInstance() {
    return g_accountService;
}

Account* AccountService::findAccount(const wchar_t* accountName) {
    for(Account* account : m_accounts) {
        if(wcscmp(accountName, account->name()) == 0)
            return account;
    }
    return nullptr;
}

Account* AccountService::createAccount(const wchar_t *accountName, const wchar_t *accountPassword) {
    //TODO(CGR): check if account already exists
    unsigned long accountId = m_accounts.size() + 1;
    auto* newAccount = new Account(accountId, accountName, L"");
    m_accounts.push_back(newAccount);
    return newAccount;
}

Player* AccountService::createPlayerFromAccount(Account &account) {
    Player* newPlayer = new Player(account);
    m_players.push_back(newPlayer);
    return newPlayer;
}

Account *AccountService::logAccount(ClientSession &clientSession, const wchar_t *accountName, const wchar_t *accountPassword) {
    Account *account = findAccount(accountName);
    return account;
}