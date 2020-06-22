//
// Created by CGR on 18/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_CAROM_MESSAGING_H__
#define __OPEN_CAROM3D_SERVER_CAROM_MESSAGING_H__

#include <core/util/types.h>
#include <business/api/constants.h>

namespace business {

#pragma pack(push, 1)

    //Received
    struct LoginActionData {
        const wchar_t username[21];
        const wchar_t password[13];
        const wchar_t country[4];
        const unsigned char unk[26];
        const unsigned char unk2[20];
        unsigned long server;
    };

    struct CreateRoomActionData {
        wchar_t roomTitle[ROOM_TITLE_MAX_LEN + 1];
        wchar_t roomPassword[ROOM_PASSWORD_MAX_LEN + 1];
        u32 unk0x0A;
        u32 roomType;
        u32 unk0x00;
        u32 difficulty;
        u32 levelLimit;
        u32 gameType;
        u32 matchType;
        int caneys;
    };

    struct JoinRoomActionData {
        wchar_t roomTitle[ROOM_TITLE_MAX_LEN + 1];
        wchar_t roomPassword[ROOM_PASSWORD_MAX_LEN + 1];
    };

    struct RoomSlotModificationActionData {
        int slotNumber;
        u32 function;
    };

    //To send
    struct LoginResult {
        long result;
        wchar_t playerName[PLAYER_NAME_MAX_LEN + 1];
        unsigned long accountNumber;
        unsigned long accountLevel;
    };

    struct ChannelPlayer {
        unsigned long accountNumber;
        wchar_t playerName[PLAYER_NAME_MAX_LEN + 1];
        bool inGuild;
        unsigned long level;
        unsigned long highestRating;
        unsigned long wins; //related to stars
        unsigned long chalks;
        unsigned long unk;
        bool hasPicture;
        wchar_t country[3];
        long caneys;
    };

    struct PrivateMessage {
        wchar_t playerName[PLAYER_NAME_MAX_LEN + 1];
        wchar_t message[];
    };

    //

    struct SlotModificationResultData {
        int listId;
        int slotNumber;
        int function;
    };

    struct PlayerStats {
        unsigned short doublePointsItemCount;
        unsigned long vipItemTime;
        unsigned char unk06;
        unsigned char power;
        unsigned char powerRange;
        unsigned char control;
        unsigned char sideSpin;
        unsigned char topSpin;
        unsigned char bottomSpin;
        unsigned char ps;
        unsigned char ms;
        short availableStatPoints;
        unsigned long unk11;
        unsigned long cueId;
        unsigned char jewel_ruby;
        unsigned char jewel_zaphira;
        unsigned char jewel_opal;
        unsigned char jewel_emerald;
        unsigned char jewel_onix;
        unsigned char jewel_topaz;
        unsigned long unk1f;
        unsigned char unk23;
        unsigned char resetStatsItemCount;
        unsigned short doubleCaneysItemCount;
        int caneys;
        wchar_t country[3];
    };

    struct RoomSlotInfoData {
        int playerListIndex[30];
        int slotState[30];
    };


    struct CreatedRoomData {
        wchar_t roomTitle[ROOM_TITLE_MAX_LEN + 1];  //+0x00
        u32 unk52; //+0x52
        u32 roomType; //+0x56
        u32 unk5A; //+0x5A
        u32 unk5E; //+0x5E
        u32 gameType; //+0x62
        u32 matchType; //+0x66
        u32 difficulty; //+0x6A
        RoomSlotInfoData slotInfos; //+0x6E
        wchar_t roomMaster[PLAYER_NAME_MAX_LEN + 1]; //+0x15E
        u32 unk188; //+0x188
    };

    struct JoinedRoomData {
        wchar_t roomTitle[ROOM_TITLE_MAX_LEN + 1];  //+0x00
        u32 unk52; //+0x52
        u32 gameType; //+0x56
        u32 roomType; //+0x5A
        u32 state; //+0x5E
        u32 matchType; //+0x62
        u32 difficulty; //+0x66
        RoomSlotInfoData slotInfos; //+0x6A
        u32 straightWins; //+0x15A
        wchar_t roomMaster[PLAYER_NAME_MAX_LEN + 1]; //+0x15D
        u32 unk188; //+0x188
    };

    struct RoomInfoActionData {
        u32 roomId;
        u32 difficulty;
        wchar_t roomName[ROOM_TITLE_MAX_LEN + 1];
        u32 playersIn;
        u32 maxPlayers;
        u32 roomType;
        u32 u;
        u32 levelLimit;
        u32 gameType;
        u32 matchType;
        u32 roomState;
        wchar_t roomMaster[PLAYER_NAME_MAX_LEN + 1];
        u32 straightWins;
        u32 caneys;
    };

    struct RoomPlayerData {
        wchar_t id[PLAYER_NAME_MAX_LEN + 1];  //+0x00
        wchar_t guild[GUILD_NAME_MAX_LEN + 1]; //+0x2A
        u32 level; //+0x4C
        int listIndex; //+0x50
        u8 unk84; //+0x54
        u32 highestRating; //+0x55
        u32 gameTypeRating; //+0x59
        u32 totalPoint; //+0x5D
        u32 wins; //+0x61
        u32 loses; //+0x65
        u32 chalks; //+0x69
        u32 unk6D; //+0x6D
        bool hasPicture; //+0x71
        wchar_t country[3]; //+0x72
        u8 unk120; //+0x78
        u8 unk121; //+0x79
        u32 accountNumber; //+0x7A
        u32 drops; //+0x7E
    //        { TODO(CGR): Identify structure
        u32 unk130; //+0x82
        u32 cueId; //+0x86
        u8 jewelRuby; //+0x8A
        u8 jewelZaphira; //+0x8B
        u8 jewelOpal; //+0x8C
        u8 jewelEmerald; //+0x8D
        u8 jewelOnix; //+0x8E
        u8 jewelTopaz; //+0x8F
        u32 unk144; //+0x90
    //    }
        u8 unk145; //+0x94
        u32 caneys; //+0x95
        u32 unused99;
        u32 charGender; //+0x9D - 1: Male, 2: Female
        u32 unusedA1;
        u32 unkA5;
        u32 unusedA9;
        u32 unkAD; //extra content count (TODO(CGR): identify content type
    //        { TODO(CGR): identify this struct
        u8 unk177; //+0xB1
        u8 power; //+0xB2
        u8 power_range; //+0xB3
        u8 control; //+0xB4
        u8 sideSpin; //+0xB5
        u8 topSpin; //+0xB6
        u8 backSpin; //+0xB7
        u8 ps; //+0xB8
        u8 ms; //+0xB9
        u16 availableStatPoints; //+0xBA
    //        }
    };

    struct RoomPlayerItemData {
        int listIndex;
        bool hasDoublePoints;
        bool hasVip;
        bool hasSafetyPoints;
        bool hasDoubleCaneys;
    };

    struct PlayerProfileData {
        u32 playerNumber; //+0x00
        wchar_t playerName[21]; //0x04
        u32 serverLoggedOn; //+0x2E
        wchar_t loggedServerName[41]; //+0x32
        wchar_t unk84[33]; //+0x84
        wchar_t unkc6[33]; //+0xC6
        wchar_t playerGuild[17]; //+0x108
        u32 loginState; //+0x12A
        u32 highestRating; //+0x12E
        u32 totalPoints; //+0x132
        u32 seasonPoints; //+0x136
        u32 totalGameCount; //+0x13A
        u32 wins; //+0x13E
        u32 loses; //+0x142
        u32 streakWins; //+0x146
        u32 chalks; //+0x14A
        u32 tournamentWins; //+0x14E
        u32 tournamentJoins; //+0x152
        u32 level; //+0x156
        wchar_t country[3]; //+0x15A
        wchar_t lastLogTime[20]; //160
        u32 levelMinPoints; //+0x188
        u32 levelMaxPoints; //+0x18C
        u32 rating[7]; //+0x190//TODO(CGR): change size to a constant value like: MAX_GAME_TYPES
        wchar_t ip[16]; //+0x1AC
        u32 serverId; //+0x1CC
        int caneys; //+0x1D0
    };

    struct UserPrivateMessage {
        wchar_t playerName[PLAYER_NAME_MAX_LEN + 1];
        wchar_t message[];
    };

    struct UserInvite {
        wchar_t playerName[PLAYER_NAME_MAX_LEN + 1];
        RoomInfoActionData roomInfo;
        wchar_t roomPassword[ROOM_PASSWORD_MAX_LEN + 1];
    };


#pragma pack(pop)

}

#endif //__OPEN_CAROM3D_SERVER_CAROM_MESSAGING_H__
