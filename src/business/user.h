//
// Created by CGR on 17/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_USER_H__
#define __OPEN_CAROM3D_SERVER_USER_H__

class ClientSession;
class Account;
class Player;
class Channel;

class User {
    ClientSession& m_clientSession;
    Account& m_account;
    Player& m_player;
    Channel* m_channelIn;

public:
    User(ClientSession& session, Account& account, Player& player);

    void setChannelIn(Channel* channel);

    ClientSession& client() const { return m_clientSession; }
    Account& account() const { return m_account; }
    Player& player() const { return m_player; }
    Channel* channelIn() const { return m_channelIn; }

};


#endif