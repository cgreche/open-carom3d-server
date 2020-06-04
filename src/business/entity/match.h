//
// Created by CGR on 22/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MATCH_H__
#define __OPEN_CAROM3D_SERVER_MATCH_H__

#include "room.h"

//TODO(CGR): implement

namespace business {

#define MAX_TEAMS 6
#define MAX_PLAYERS_PER_TEAM 6

    class Match {
    public:
        struct MatchState {
            int m_currentScore[MAX_TEAMS];
            int currentPlayer;
            int status;
            u32 result;
        };

    protected:
        const Room &m_room;
        u32 m_id;
        u32 m_randomSeed;
        int m_teamCount;
        int m_playersPerTeam;

    public:
        Match(int matchId, const Room &room, u32 matchRandomSeed);

        u32 id() const { return m_id; }
        u32 randomSeed() const { return m_randomSeed; }
        int teamCount() const { return m_teamCount; }
        int playersPerTeam() const { return m_playersPerTeam; }
        const wchar_t* matchDescription() const { return L""; } //TODO(CGR): todo
        const Room &room() { return m_room; }
    };

}

#endif //__OPEN_CAROM3D_SERVER_MATCH_H__
