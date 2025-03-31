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
        /**
         * @brief Generates an RSA asymmetric key pair of the specified size.
         *
         * This method generates an RSA private/public key pair using OpenSSL's EVP API.
         * It initializes the RSA context, sets the key size, and generates the key pair.
         *
         * @param size The size of the RSA key in bits (e.g., 2048, 4096).
         *
         * @return asymmetric_key_t A shared pointer to the generated `EVP_PKEY` key pair.
         *
         * @throws std::invalid_argument If the key size is 0.
         * @throws std::runtime_error If any error occurs during the key generation process such as:
         * - Failure in initializing the RSA context.
         * - Failure in initializing the keygen.
         * - Failure in setting the key length.
         * - Failure in generating the key pair.
         */
        static asymmetric_key_t generate_key(std::size_t size_in_bits);

        /**
         * @brief Converts a public key in PEM format to an `EVP_PKEY` object.
         *
         * This method takes a public key represented as a PEM-encoded string and converts it
         * into an `EVP_PKEY` object. The key is read into a BIO (Basic I/O) buffer, which is then
         * used to parse the PEM-encoded key.
         *
         * Example PEM format for an RSA public key:
         *
         * @code
         * -----BEGIN PUBLIC KEY-----
         *             KEY
         * -----END PUBLIC KEY-----
         * @endcode
         *
         * @param key_str The PEM-encoded public key as a string.
         *
         * @return asymmetric_key_t A shared pointer to the `EVP_PKEY` object representing the public key.
         *
         * @throws std::invalid_argument If the key is empty.
         * @throws std::runtime_error If any error occurs during initializing the BIO.
         */
        static asymmetric_key_t to_public_key(const std::string& key_str);

        /**
         * @brief Extracts the public key from an asymmetric key and returns it in PEM format.
         *
         * This method extracts the public key from an `asymmetric_key_t` (which is an `EVP_PKEY`
         * object) and returns it as a PEM-encoded string. It writes the public key to a BIO (Basic I/O)
         * in PEM format, reads the raw PEM data, and then returns it as a `std::string`.
         *
         * Example PEM format for an RSA public key:
         *
         * @code
         * -----BEGIN PUBLIC KEY-----
         *             KEY
         * -----END PUBLIC KEY-----
         * @endcode
         *
         * @param key A shared pointer to the `EVP_PKEY` object that contains the asymmetric key.
         *
         * @return std::string A PEM-encoded string representing the public key.
         *
         * @throws std::invalid_argument If the key is `nullptr`.
         * @throws std::runtime_error If any error occurs during the extraction process, such as:
         * - Failure in initializing the BIO (Basic I/O).
         * - Failure in writing the public key to the BIO.
         */
        static std::string extract_public_key(asymmetric_key_t key);

        /**
         * @brief Encrypts data using RSA with OAEP(Optimal Asymmetric Encryption Padding) padding.
         *
         * This method encrypts the provided data using RSA encryption with Optimal Asymmetric
         * Encryption Padding (OAEP) and the specified public key. It uses OpenSSL's EVP API to
         * perform the encryption operation.
         *
         * The method checks that the key is valid before attempting the encryption, initializes the RSA context,
         * sets the padding scheme, and then performs the encryption.
         *
         * @param key A shared pointer to the public `EVP_PKEY` object used for encryption.
         * @param bytes A vector of bytes to be encrypted.
         *
         * @return std::vector<unsigned char> A vector containing the encrypted data.
         *
         * @throws std::invalid_argument If the key is `nullptr`.
         * @throws std::runtime_error If any error occurs during the encryption process, such as:
         * - Failure in initializing the RSA context.
         * - Failure in setting encryption padding.
         * - Failure during the encryption operation.
         */
        static std::vector<unsigned char> encrypt(asymmetric_key_t key, const std::vector<unsigned char>& bytes);

        /**
         * @brief Decrypts data using RSA with OAEP padding.
         *
         * This method decrypts the provided encrypted data using RSA decryption with Optimal
         * Asymmetric Encryption Padding (OAEP) and the specified private key. It uses OpenSSL's
         * EVP API to perform the decryption operation.
         *
         * The method checks that the key is valid before attempting the decryption, initializes the RSA context,
         * sets the padding scheme, and then performs the decryption.
         *
         * @param key A shared pointer to the private `EVP_PKEY` object used for decryption.
         * @param encrypted_bytes A vector of bytes to be decrypted.
         *
         * @return std::vector<unsigned char> A vector containing the decrypted data.
         *
         * @throws std::invalid_argument If the key is `nullptr`.
         * @throws std::runtime_error If any error occurs during the decryption process, such as:
         * - Failure in initializing the RSA context.
         * - Failure in setting decryption padding.
         * - Failure during the decryption operation.
         */
        static std::vector<unsigned char> decrypt(asymmetric_key_t key, const std::vector<unsigned char>& encrypted_bytes);
    };
}

#endif  // ENCRYPTION_RSA_H
