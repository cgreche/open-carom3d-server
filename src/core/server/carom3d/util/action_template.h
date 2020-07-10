//
// Created by CGR on 22/06/2020.
//

#ifndef __OPEN_CAROM3D_SERVER_ACTION_TEMPLATE_H__
#define __OPEN_CAROM3D_SERVER_ACTION_TEMPLATE_H__

namespace core {

    class ActionTemplate {
    public:
        virtual ~ActionTemplate() {}
        virtual const ActionData data() = 0;
    };

}

#endif /*__OPEN_CAROM3D_SERVER_ACTION_TEMPLATE_H__*/