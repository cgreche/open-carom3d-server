//
// Created by CGR on 13/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTION_H__
#define __OPEN_CAROM3D_SERVER_ACTION_H__

#include <vector>

class ClientSession;

class ActionData {
    int m_id;
    std::vector<unsigned char> m_content;

public:
    ActionData(int id, unsigned char *data, unsigned int dataLen) {
        this->m_id = id;
        this->m_content.assign(data,data + dataLen);
    }

    int id() const { return m_id; }
    const std::vector<unsigned char>& data() const { return m_content; }
};

class Action {
public:
    virtual ~Action() { };
    virtual bool validate(ActionData &action) = 0;
    virtual void execute(ActionData &action, ClientSession &client) = 0;
};

template <class T>
class AbstractAction : public Action {
public:
    bool validate(ActionData &action) override {
        return true;
    }

    void execute(ActionData &action, ClientSession &client) override {
        const T* typedData = reinterpret_cast<const T*>(action.data().data());
        execute(action, client, typedData);
    }

    T* cast(ActionData &action) {
        return (T*)action.data();
    }

    virtual void execute(ActionData &action, ClientSession &client, const T* data) = 0;
};

#endif //__OPEN_CAROM3D_SERVER_ACTION_H__
