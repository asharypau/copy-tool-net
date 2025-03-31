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
        /**
         * @brief Generates a random AES symmetric key of the specified size.
         *
         * @param size The length of the AES key in bytes. Common key sizes are:
         * - 16 bytes (128-bit AES)
         * - 24 bytes (192-bit AES)
         * - 32 bytes (256-bit AES)
         *
         * @return Encryption::symmetric_key_t A vector containing the randomly generated AES key.
         *
         * @throws std::invalid_argument If the key size is zero.
         * @throws std::runtime_error If the key generation process fails (e.g., OpenSSL's `RAND_bytes` fails).
         */
        static Encryption::symmetric_key_t generate_key(std::size_t size);

        /**
         * @brief Encrypts data using AES-256-ECB mode.
         *
         * This method encrypts the given plaintext using AES-256 in ECB mode with the provided symmetric key.
         *
         * @param key A vector containing the AES symmetric key (must be 32 bytes for AES-256).
         * @param data The plaintext data to be encrypted.
         *
         * @return std::vector<unsigned char> A vector containing the encrypted data.
         *
         * @throws std::invalid_argument If the key is empty.
         * @throws std::runtime_error If an error occurs during the encryption process, such as:
         * - Failure to initialize the AES encryption context.
         * - Failure during the encryption operation.
         * - Failure to finalize the encryption.
         */
        static std::vector<unsigned char> encrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& data);

        /**
         * @brief Decrypts AES-256-ECB encrypted data.
         *
         * This method decrypts data using AES-256 in ECB mode with the provided symmetric key.
         *
         * @param key A vector containing the AES symmetric key (must be 32 bytes for AES-256).
         * @param encrypted_data The ciphertext data to be decrypted.
         *
         * @return std::vector<unsigned char> A vector containing the decrypted data.
         *
         * @throws std::invalid_argument If the key is empty.
         * @throws std::runtime_error If an error occurs during the decryption process, such as:
         * - Failure to initialize the AES decryption context.
         * - Failure during the decryption operation.
         * - Failure to finalize the decryption (potentially indicating incorrect key or corrupted ciphertext).
         */
        static std::vector<unsigned char> decrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& encrypted_data);
    };
}

#endif  // ENCRYPTION_AES_H
