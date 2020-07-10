//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_MESSAGE_PARSER_H__
#define __OPEN_CAROM3D_SERVER_MESSAGE_PARSER_H__

#include <vector>
#include "action.h"
#include "crypto.h"

namespace core {

    struct ParsedDataResultInfo {
        std::vector<ActionData> parsedActions;
        unsigned int parsedTotalLen;
    };

    class Carom3DMessageParser {
    public:
        ParsedDataResultInfo parseMessageData(CryptoContext &cryptoCtx, unsigned char *data, unsigned int dataLen);
    };

}

#endif //__OPEN_CAROM3D_SERVER_MESSAGE_PARSER_H__
