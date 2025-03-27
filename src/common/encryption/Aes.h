#ifndef ENCRYPTION_AES_H
#define ENCRYPTION_AES_H

#include <openssl/crypto.h>
#include <openssl/rand.h>

#include <vector>

#include "Constants.h"

namespace Encryption
{
    class Aes
    {
    public:
        static Encryption::symmetric_key_t generate_key(size_t size);
        static std::vector<unsigned char> encrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& data);
        static std::vector<unsigned char> decrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& encrypted_data);
    };
}

#endif  // ENCRYPTION_AES_H
