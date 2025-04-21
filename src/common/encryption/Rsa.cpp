#include "Rsa.h"

#include <cstddef>
#include <stdexcept>

using namespace Encryption;

asymmetric_key_t Rsa::generate_key(std::size_t size)
{
    if (size == 0)
    {
        throw std::invalid_argument("RSA key size cannot be zero");
    }

    // Initialize the RSA context
    std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)> ctx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr), &EVP_PKEY_CTX_free);
    if (!ctx)
    {
        throw std::runtime_error("Error initializing RSA context during key generation");
    }

    // Initialize key generation
    if (EVP_PKEY_keygen_init(ctx.get()) <= 0)
    {
        throw std::runtime_error("Error initializing RSA keygen");
    }

    // Set the RSA key size
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx.get(), size) <= 0)
    {
        throw std::runtime_error("Error setting RSA key length");
    }

    // Key generation
    EVP_PKEY* raw_key = nullptr;
    if (EVP_PKEY_keygen(ctx.get(), &raw_key) <= 0)
    {
        throw std::runtime_error("Error generating RSA key");
    }

    return std::shared_ptr<EVP_PKEY>(raw_key, &EVP_PKEY_free);
}

asymmetric_key_t Rsa::to_public_key(const std::string& key_str)
{
    if (key_str.empty())
    {
        throw std::invalid_argument("Input string cannot be empty during converting key_str to RSA asymmetric_key_t");
    }

    // Create BIO (Basic I/O) from string
    std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new_mem_buf(key_str.data(), static_cast<int>(key_str.size())), &BIO_free);
    if (!bio)
    {
        throw std::runtime_error("Error initializing BIO (Basic I/O) during converting key_str to RSA asymmetric_key_t");
    }

    // Read public key from BIO (Basic I/O)
    EVP_PKEY* raw_key = PEM_read_bio_PUBKEY(bio.get(), nullptr, nullptr, nullptr);

    return std::shared_ptr<EVP_PKEY>(raw_key, &EVP_PKEY_free);
}

std::string Rsa::extract_public_key(asymmetric_key_t key)
{
    if (!key)
    {
        throw std::invalid_argument("The RSA key must be exist during public_key extraction");
    }

    // Create BIO (Basic I/O)
    std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), &BIO_free);
    if (!bio)
    {
        throw std::runtime_error("Error initializing BIO (Basic I/O) during RSA public_key extraction");
    }

    // Write the public key to the BIO in PEM format
    if (PEM_write_bio_PUBKEY(bio.get(), key.get()) <= 0)
    {
        throw std::runtime_error("Error writing public_key to the BIO (Basic I/O) during RSA public_key extraction");
    }

    // Read the PEM data from the BIO
    char* public_raw_key;
    long public_raw_key_length = BIO_get_mem_data(bio.get(), &public_raw_key);
    std::string public_key = std::string(public_raw_key, public_raw_key_length);

    return std::move(public_key);
}

std::vector<unsigned char> Rsa::encrypt(asymmetric_key_t key, const std::vector<unsigned char>& bytes)
{
    if (!key)
    {
        throw std::invalid_argument("The key must be exist during RSA encryption");
    }

    // Initialize the RSA context
    std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)> ctx(EVP_PKEY_CTX_new(key.get(), nullptr), &EVP_PKEY_CTX_free);
    if (!ctx)
    {
        throw std::runtime_error("Error initializing RSA context during encryption");
    }

    // Initialize encryption operation using the public key (RSA encryption with OAEP(Optimal Asymmetric Encryption Padding) padding)
    if (EVP_PKEY_encrypt_init(ctx.get()) <= 0)
    {
        throw std::runtime_error("Error initializing RSA encryption");
    }

    // Set padding for RSA encryption
    if (EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_OAEP_PADDING) <= 0)
    {
        throw std::runtime_error("Error setting RSA encryption padding");
    }

    // Encrypt data using RSA
    std::vector<unsigned char> encrypted_data(EVP_PKEY_size(key.get()));
    std::size_t encrypted_data_length = encrypted_data.size();
    if (EVP_PKEY_encrypt(ctx.get(), encrypted_data.data(), &encrypted_data_length, bytes.data(), bytes.size()) <= 0)
    {
        throw std::runtime_error("Error during RSA encryption");
    }

    encrypted_data.resize(encrypted_data_length);

    return std::move(encrypted_data);
}

std::vector<unsigned char> Rsa::decrypt(asymmetric_key_t key, const std::vector<unsigned char>& encrypted_bytes)
{
    if (!key)
    {
        throw std::invalid_argument("The key must be exist during RSA decryption");
    }

    // Initialize the RSA context
    std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)> ctx(EVP_PKEY_CTX_new(key.get(), nullptr), &EVP_PKEY_CTX_free);
    if (!ctx)
    {
        throw std::runtime_error("Error initializing RSA context during decryption");
    }

    // Initialize decryption operation (RSA decryption with OAEP(Optimal Asymmetric Encryption Padding) padding)
    if (EVP_PKEY_decrypt_init(ctx.get()) <= 0)
    {
        throw std::runtime_error("Error initializing RSA decryption");
    }

    // Set padding for RSA decryption
    if (EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_OAEP_PADDING) <= 0)
    {
        throw std::runtime_error("Error setting RSA decryption padding");
    }

    // Decrypt data using RSA
    std::vector<unsigned char> decrypted_data(EVP_PKEY_size(key.get()));
    std::size_t decrypted_data_length = decrypted_data.size();
    if (EVP_PKEY_decrypt(ctx.get(), decrypted_data.data(), &decrypted_data_length, encrypted_bytes.data(), encrypted_bytes.size()) <= 0)
    {
        throw std::runtime_error("Error during RSA decryption");
    }

    decrypted_data.resize(decrypted_data_length);

    return std::move(decrypted_data);
}
