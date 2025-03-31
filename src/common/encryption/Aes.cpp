#include "Aes.h"

#include <memory>
#include <stdexcept>

using namespace Encryption;

Encryption::symmetric_key_t Aes::generate_key(std::size_t size)
{
    if (size == 0)
    {
        throw std::invalid_argument("AES key size cannot be zero");
    }

    // Create symmetric key
    Encryption::symmetric_key_t key(size);

    // Fill the key with random bytes
    if (RAND_bytes(key.data(), size) != 1)
    {
        throw std::runtime_error("Error generating AES key");
    }

    return std::move(key);
}

std::vector<unsigned char> Aes::encrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& data)
{
    if (key.empty())
    {
        throw std::invalid_argument("The key must be exist during AES encryption");
    }

    // Initialize the context
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
    if (!ctx)
    {
        throw std::runtime_error("Error initializing AES context during encryption");
    }

    // Initialize AES encryption
    if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_ecb(), nullptr, key.data(), nullptr) != 1)
    {
        throw std::runtime_error("Error initializing AES encryption");
    }

    // Encrypt the data
    int length = 0;
    std::vector<unsigned char> encrypted_data(data.size() + EVP_CIPHER_block_size(EVP_aes_256_ecb()));
    if (EVP_EncryptUpdate(ctx.get(), encrypted_data.data(), &length, data.data(), data.size()) != 1)
    {
        throw std::runtime_error("Error during AES encryption");
    }

    // Finish the encryption process
    int final_length = 0;
    if (EVP_EncryptFinal_ex(ctx.get(), encrypted_data.data() + length, &final_length) != 1)
    {
        throw std::runtime_error("Error during AES encryption finalization");
    }

    encrypted_data.resize(length + final_length);

    return std::move(encrypted_data);
}

std::vector<unsigned char> Aes::decrypt(const Encryption::symmetric_key_t& key, const std::vector<unsigned char>& encrypted_data)
{
    if (key.empty())
    {
        throw std::invalid_argument("The key must be exist during AES decryption");
    }

    // Initialize the context
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
    if (!ctx)
    {
        throw std::runtime_error("Error initializing AES context during decryption");
    }

    // Initialize AES decryption
    if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_ecb(), nullptr, key.data(), nullptr) != 1)
    {
        throw std::runtime_error("Error initializing AES decryption");
    }

    int length = 0;
    std::vector<unsigned char> decrypted_data(encrypted_data.size());

    // Decrypt the data
    if (EVP_DecryptUpdate(ctx.get(), decrypted_data.data(), &length, encrypted_data.data(), encrypted_data.size()) != 1)
    {
        throw std::runtime_error("Error during AES decryption");
    }

    int final_length = 0;

    // Finish the decryption process
    if (EVP_DecryptFinal_ex(ctx.get(), decrypted_data.data() + length, &final_length) != 1)
    {
        throw std::runtime_error("Error during AES encryption finalization");
    }

    decrypted_data.resize(length + final_length);

    return std::move(decrypted_data);
}
