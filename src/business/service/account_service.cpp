//
// Created by CGR on 16/05/2020.
//

#include <algorithm>
#include <core/server/client_session.h>
#include "account_service.h"
#include "business/entity/player.h"

namespace business {

    static AccountService g_accountService;

    static std::map<std::wstring, Account*> g_accounts;
    static std::map<u32, Player*> g_players;

    AccountService &AccountService::getInstance() {
        return g_accountService;
    }

    Account *AccountService::createAccount(const wchar_t *accountName, const wchar_t *accountPassword) {
        //TODO(CGR): check if account already exists
        unsigned long accountId = g_accounts.size() + 1;
        auto *newAccount = new Account(accountId, accountName, L"");

        std::wstring accountKey = accountName;
        std::transform(accountKey.begin(), accountKey.end(), accountKey.begin(),
            [](unsigned char c) { return std::tolower(c); });
        g_accounts[accountKey] = newAccount;
        return newAccount;
    }

    Player *AccountService::createPlayerFromAccount(Account &account) {
        Player *newPlayer = new Player(account);
        g_players[account.id()] = newPlayer;
        return newPlayer;
    }

    Account *AccountService::logAccount(const wchar_t *accountName, const wchar_t *accountPassword) {
        Account *account = findAccount(accountName);
        return account;
    }

    Account* AccountService::findAccount(const wchar_t* accountName) {
        std::wstring accountId = accountName;
        std::transform(accountId.begin(), accountId.end(), accountId.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return g_accounts[accountId];
    }

    Player* AccountService::getPlayer(Account& account) {
        return g_players[account.id()];
    }

}