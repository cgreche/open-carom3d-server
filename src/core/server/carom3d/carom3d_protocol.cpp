//
// Created by CGR on 09/06/2020.
//

#include <vector>
#include <ctime>
#include "carom3d_protocol.h"
#include "carom3d_user_session.h"
#include "action.h"


namespace core {

    Carom3DProtocol::Carom3DProtocol() {
        m_userActionMap = nullptr;
    }

    ClientSession* Carom3DProtocol::createSession(nettools::ntConnection& ntClient, Server& server) {
        return new Carom3DUserSession(ntClient, server);
    }

    void Carom3DProtocol::onMessageReceived(ClientSession& session) {
        Carom3DUserSession& user = (Carom3DUserSession&)session;
        user.update();

        u32 dataLen = user.pendingReadDataSize();
        ParsedDataResultInfo parsedData = m_messageParser.parseMessageData(user.inDataCryptoCtx(), user.pendingReadData(), user.pendingReadDataSize());

        unsigned int parsedLen = parsedData.parsedTotalLen;
        user.discardReadPendingData(parsedLen);

        user.appendActions(parsedData.parsedActions);
        processUserActions(user);
    }

    void Carom3DProtocol::onMessageSent(ClientSession& session) {
        Carom3DUserSession& user = (Carom3DUserSession&)session;

        u32 dataLen = user.pendingReadDataSize();
        ParsedDataResultInfo parsedData = m_messageParser.parseMessageData(user.inDataCryptoCtx(), user.pendingReadData(), user.pendingReadDataSize());

        unsigned int parsedLen = parsedData.parsedTotalLen;
        user.discardReadPendingData(parsedLen);

        user.appendActions(parsedData.parsedActions);
        processUserActions(user);
    }

    void Carom3DProtocol::closeSession(ClientSession& session) {
        delete &session;
    }

    void Carom3DProtocol::setUserActionMap(std::map<int, Action*>* actionMap) {
        m_userActionMap = actionMap;
    }

    void Carom3DProtocol::processUserActions(Carom3DUserSession& user) {
        if(nullptr == m_userActionMap)
            return;

        const std::vector<ActionData>& actions = user.pendingActions();
        if(actions.empty())
            return;

        for(auto actionData : actions) {
            onUserAction(user, actionData);
        }
        user.clearPendingActions();
    }

    void Carom3DProtocol::onUserAction(Carom3DUserSession& session, const ActionData& actionData) {
        Action* action = (*m_userActionMap)[actionData.id()];
        if(action != nullptr) {
            if(action->validate(actionData)) {
                action->execute(actionData, session);
            }
        }
        else {
            onUnhandledUserAction(session, actionData);
        }
    }

    void Carom3DProtocol::onUnhandledUserAction(Carom3DUserSession& session, const ActionData& actionData) {
        //TODO: Invalid Action
        int a = actionData.id();
        printf("Unhandled action: %x - %d\n", a, actionData.data().size());
    }

}