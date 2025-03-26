#ifndef ENCRYPTION_AES_H
#define ENCRYPTION_AES_H

#include <openssl/rand.h>

#include <utility>
#include <vector>

#include "Constants.h"

namespace Encryption
{
    class Aes
    {
    public:
        static Encryption::symmetric_key_t generate_key(size_t size)
        {
            Encryption::symmetric_key_t key(size);

            if (RAND_bytes(key.data(), size) != 1)
            {
                Encryption::symmetric_key_t();
            }

            return std::move(key);
        }

        static std::vector<unsigned char> encrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& data)
        {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx)
            {
                return std::vector<unsigned char>();
            }

            if (EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), nullptr, key.data(), nullptr) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);

                return std::vector<unsigned char>();
            }

            int length = 0;
            std::vector<unsigned char> encrypted_data(data.size() + EVP_CIPHER_block_size(EVP_aes_256_ecb()));
            if (EVP_EncryptUpdate(ctx, encrypted_data.data(), &length, data.data(), data.size()) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);

                return std::vector<unsigned char>();
            }

            int final_length = 0;
            if (EVP_EncryptFinal_ex(ctx, encrypted_data.data() + length, &final_length) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);

                return std::vector<unsigned char>();
            }

            encrypted_data.resize(length + final_length);
            EVP_CIPHER_CTX_free(ctx);

            return std::move(encrypted_data);
        }

        static std::vector<unsigned char> decrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& encrypted_data)
        {
            EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
            if (!ctx)
            {
                return std::vector<unsigned char>();
            }

            if (EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), nullptr, key.data(), nullptr) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);

                return std::vector<unsigned char>();
            }

            int length = 0;
            std::vector<unsigned char> decrypted_data(encrypted_data.size());
            if (EVP_DecryptUpdate(ctx, decrypted_data.data(), &length, encrypted_data.data(), encrypted_data.size()) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);

                return std::vector<unsigned char>();
            }

            int final_length = 0;
            if (EVP_DecryptFinal_ex(ctx, decrypted_data.data() + length, &final_length) != 1)
            {
                EVP_CIPHER_CTX_free(ctx);

                return std::vector<unsigned char>();
            }

            decrypted_data.resize(length + final_length);
            EVP_CIPHER_CTX_free(ctx);

            return std::move(decrypted_data);
        }
    };
}

#endif  // ENCRYPTION_AES_H
