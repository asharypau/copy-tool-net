#include "Rsa.h"

#include <cstddef>

using namespace Encryption;

asymmetric_key_t Rsa::generate_key(size_t size)
{
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx)
    {
        return nullptr;
    }

    // Initialize key generation
    if (EVP_PKEY_keygen_init(ctx) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return nullptr;
    }

    // Set the RSA key size
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, size) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return nullptr;
    }

    EVP_PKEY* raw_key = nullptr;
    if (EVP_PKEY_keygen(ctx, &raw_key) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return nullptr;
    }

    EVP_PKEY_CTX_free(ctx);

    return std::shared_ptr<EVP_PKEY>(
        raw_key,
        [](EVP_PKEY* raw_ptr)
        {
            EVP_PKEY_free(raw_ptr);
        });
}

asymmetric_key_t Rsa::to_public_key(const std::string& key_str)
{
    BIO* bio = BIO_new_mem_buf(key_str.data(), static_cast<int>(key_str.size()));
    if (!bio)
    {
        return nullptr;
    }

    EVP_PKEY* raw_key = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);

    return std::shared_ptr<EVP_PKEY>(
        raw_key,
        [](EVP_PKEY* raw_ptr)
        {
            EVP_PKEY_free(raw_ptr);
        });
}

std::string Rsa::extract_public_key(asymmetric_key_t key)
{
    if (!key)
    {
        return std::string();
    }

    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio)
    {
        return std::string();
    }

    // Write the public key to the BIO in PEM format
    if (PEM_write_bio_PUBKEY(bio, key.get()) <= 0)
    {
        BIO_free(bio);

        return std::string();
    }

    // Read the PEM data from the BIO
    char* public_raw_key;
    long public_raw_key_length = BIO_get_mem_data(bio, &public_raw_key);

    std::string public_key = std::string(public_raw_key, public_raw_key_length);
    BIO_free(bio);

    return std::move(public_key);
}

std::vector<unsigned char> Rsa::encrypt(asymmetric_key_t key, const std::vector<unsigned char>& data)
{
    if (!key)
    {
        return std::vector<unsigned char>();
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key.get(), nullptr);
    if (!ctx)
    {
        return std::vector<unsigned char>();
    }

    // Initialize encryption operation (RSA encryption with OAEP padding)
    if (EVP_PKEY_encrypt_init(ctx) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return std::vector<unsigned char>();
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return std::vector<unsigned char>();
    }

    std::vector<unsigned char> encrypted_data(EVP_PKEY_size(key.get()));
    size_t encrypted_data_length = encrypted_data.size();

    if (EVP_PKEY_encrypt(ctx, encrypted_data.data(), &encrypted_data_length, data.data(), data.size()) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return std::vector<unsigned char>();
    }

    encrypted_data.resize(encrypted_data_length);
    EVP_PKEY_CTX_free(ctx);

    return std::move(encrypted_data);
}

std::vector<unsigned char> Rsa::decrypt(asymmetric_key_t key, const std::vector<unsigned char>& encrypted_data)
{
    if (!key)
    {
        return std::vector<unsigned char>();
    }

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key.get(), nullptr);
    if (!ctx)
    {
        return std::vector<unsigned char>();
    }

    // Initialize decryption operation (RSA decryption with OAEP padding)
    if (EVP_PKEY_decrypt_init(ctx) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return std::vector<unsigned char>();
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return std::vector<unsigned char>();
    }

    std::vector<unsigned char> decrypted_data(EVP_PKEY_size(key.get()));
    size_t decrypted_data_length = decrypted_data.size();

    if (EVP_PKEY_decrypt(ctx, decrypted_data.data(), &decrypted_data_length, encrypted_data.data(), encrypted_data.size()) <= 0)
    {
        EVP_PKEY_CTX_free(ctx);

        return std::vector<unsigned char>();
    }

    decrypted_data.resize(decrypted_data_length);
    EVP_PKEY_CTX_free(ctx);

    return std::move(decrypted_data);
}
