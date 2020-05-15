//
// Created by CGR on 14/05/2020.
//

#include "message_parser.h"

ParsedDataResultInfo MessageParser::parseMessageData(CryptoContext &cryptoCtx, unsigned char *data, unsigned int dataLen) {
    ParsedDataResultInfo info;
    int i = 0;
    unsigned int parsedLen = 0;
    unsigned int unparsedLen = dataLen;
    while(unparsedLen >= 8) {
        unsigned char *d = &data[parsedLen];
        int actionId = *(int*)&d[0];
        unsigned int actionDataLen = *(unsigned int*)&d[4];
        cryptoCtx.crypt((unsigned char*)&actionId, 4);
        cryptoCtx.crypt((unsigned char*)&actionDataLen, 4);
        if((unparsedLen-8) < actionDataLen) {
            //Couldn't parse
            //TODO: set error to info
            return info;
        }
        d += 8;
        cryptoCtx.crypt(d,actionDataLen);
        info.parsedActions.push_back(ActionData(actionId,d,actionDataLen));
        parsedLen += 8 + actionDataLen;
        unparsedLen = dataLen - parsedLen;
        cryptoCtx.update();
    }
    info.parsedTotalLen = parsedLen;
    return info;
}

