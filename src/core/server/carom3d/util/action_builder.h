//
// Created by CGR on 21/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTION_BUILDER_H__
#define __OPEN_CAROM3D_SERVER_ACTION_BUILDER_H__

#include <string>
#include <vector>
#include <core/util/types.h>

#include <core/server/carom3d/action.h>

namespace core {

    class ActionBuilder {

        int m_id;
        std::vector<u8> m_data;

        template <class T> ActionBuilder& _add(T& data) {
            m_data.insert(m_data.end(), (u8*)&data, (u8*)&data + sizeof(T));
            return *this;
        }

    public:
        ActionBuilder(int actionId) { m_id = actionId; }

        ActionBuilder& add(int data) { return _add<int>(data); }
        ActionBuilder& add(unsigned int data) { return _add<unsigned int>(data); }
        ActionBuilder& add(u8 data) { return _add<u8>(data); }
        ActionBuilder& add(s8 data) { return _add<s8>(data); }
        ActionBuilder& add(u16 data) { return _add<u16>(data); }
        ActionBuilder& add(s16 data) { return _add<s16>(data); }
        ActionBuilder& add(u32 data) { return _add<u32>(data); }
        ActionBuilder& add(s32 data) { return _add<s32>(data); }

        ActionBuilder& add(const wchar_t* data) {
            u32 len = ::wcslen(data) + 1;
            len *= 2;
            add(data, len);
            return *this;
        }

        ActionBuilder& add(const char* data) {
            u32 len = ::strlen(data) + 1;
            return add(data, len);
        }

        ActionBuilder& add(const std::string data) {
            return add(data.data(), data.size()+1);
        }

        ActionBuilder& add(const std::wstring data) {
            return add(data.data(), (data.size()+1)*2);
        }

        ActionBuilder& add(const void* data, u32 len) {
            m_data.insert(m_data.end(), (u8*)data, (u8*)data + len);
            return *this;
        }

        ActionData build() {
            return ActionData(m_id, m_data.data(), m_data.size());
        }
    };

}

#endif /*__OPEN_CAROM3D_SERVER_ACTION_BUILDER_H__*/