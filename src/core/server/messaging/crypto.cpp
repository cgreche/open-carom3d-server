//
// Created by CGR on 13/05/2020.
//

#include "crypto.h"

#define INITIAL_CRYPTO_BYTE_VALUE 0x02

CryptoContext::CryptoContext() {
    m_cryptoByte = INITIAL_CRYPTO_BYTE_VALUE;
}

void CryptoContext::crypt(unsigned char data[], unsigned int len) {
    for(int i = 0; i < len; ++i) {
        data[i] ^= m_cryptoByte;
    }
}

void CryptoContext::update() {
    m_cryptoByte = m_cryptoByte*m_cryptoByte + m_cryptoByte*2 + m_cryptoByte/3 + 4;
}

