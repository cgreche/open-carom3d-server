//
// Created by CGR on 13/05/2020.
//

#ifndef OPEN_CAROM3D_SERVER_CRYPTO_H
#define OPEN_CAROM3D_SERVER_CRYPTO_H

namespace core {

    class CryptoContext {
        unsigned char m_cryptoByte;

    public:
        CryptoContext();

        void crypt(unsigned char data[], unsigned int len);

        void update();
    };

}

#endif //OPEN_CAROM3D_SERVER_CRYPTO_H
