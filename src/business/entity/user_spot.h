//
// Created by CGR on 19/05/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_USER_SPOT_H__
#define __OPEN_CAROM3D_SERVER_USER_SPOT_H__

namespace business {

class User;

    class UserSpot {
    public:
        virtual ~UserSpot() = default;
        virtual bool isOfType(int type) = 0;
        virtual const wchar_t* description() const = 0;
        virtual const wchar_t *name() const = 0;

        virtual int insertUser(User& user) = 0;
        virtual void removeUser(User& user) = 0;
        virtual bool isUserIn(const wchar_t* userName) = 0;
		virtual unsigned int userCount() = 0;
    };

}

#endif //__OPEN_CAROM3D_SERVER_USER_SPOT_H__
