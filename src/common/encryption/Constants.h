#ifndef ENCRYPTION_CONSTANTS_H
#define ENCRYPTION_CONSTANTS_H

#include <openssl/crypto.h>

#include <memory>
#include <vector>

namespace Encryption
{
    using asymmetric_key_t = std::shared_ptr<EVP_PKEY>;
    using symmetric_key_t = std::vector<unsigned char>;
}

#endif  // ENCRYPTION_CONSTANTS_H
