//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTION_H__
#define __OPEN_CAROM3D_SERVER_ACTION_H__

#include <vector>

namespace core {

    class ClientSession;

    class ActionData {
        int m_id;
        std::vector<unsigned char> m_content;

    public:
        ActionData(int id, const void *data, unsigned int dataLen) {
            this->m_id = id;
            this->m_content.assign((unsigned char *)data, (unsigned char*)data + dataLen);
        }

        explicit ActionData(int id) {
            this->m_id = id;
        }

        int id() const { return m_id; }

        const std::vector<unsigned char> &data() const { return m_content; }
    };

    class Action {
    public:
        virtual ~Action() = default;
        virtual bool validate(const ActionData &action) = 0;
        virtual void execute(const ActionData &action, ClientSession &client) = 0;
    };

}

#endif //__OPEN_CAROM3D_SERVER_ACTION_H__
