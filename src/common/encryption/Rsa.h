#ifndef ENCRYPTION_RSA_H
#define ENCRYPTION_RSA_H

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#include <string>
#include <vector>

#include "Constants.h"

namespace Encryption
{
    class Rsa
    {
    public:
        static asymmetric_key_t generate_key(size_t size);
        static asymmetric_key_t to_public_key(const std::string& key_str);
        static std::string extract_public_key(asymmetric_key_t key);
        static std::vector<unsigned char> encrypt(asymmetric_key_t key, const std::vector<unsigned char>& data);
        static std::vector<unsigned char> decrypt(asymmetric_key_t key, const std::vector<unsigned char>& encrypted_data);
    };
}

#endif  // ENCRYPTION_RSA_H
