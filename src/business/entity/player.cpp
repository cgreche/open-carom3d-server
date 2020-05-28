//
// Created by CGR on 16/05/2020.
//

#include "account.h"
#include "player.h"
#include <ctime>

namespace business {
    Player::Player(Account &account)
        :m_account(account) {
        m_id = account.id(); //TODO(CGR): change futurely
        m_playerName = account.name();
        m_points = 0;
        //TODO(CGR): remove
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        int random_variable = std::rand();
        m_level = 1 + random_variable%54;
    }
}