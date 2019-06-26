#include <algorithm>
#include <stdexcept>

#include <Windows.h>
#include <bcrypt.h>

#include <String.hpp>

#include "ScopeExit.hpp"
#include "Crypto.hpp"

using namespace std;
using kanan::narrow;
using kanan::widen;

#ifndef NT_SUCCESS
#define NT_SUCCESS(status) (((NTSTATUS)(status)) >= 0)
#endif

vector<uint8_t> hashData(string_view algorithm, const uint8_t* data, size_t length) {
    // Create CNG algorithm provider.
    BCRYPT_ALG_HANDLE alg{};

    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&alg, widen(algorithm).c_str(), nullptr, 0))) {
        throw runtime_error{ "Failed to open algorithm provider" };
    }

    SCOPE_EXIT([=] { BCryptCloseAlgorithmProvider(alg, 0); });

    // Get the size required for the hash object and hash result.
    ULONG numBytes{};
    ULONG objectLength{};
    ULONG hashLength{};

    if (!NT_SUCCESS(BCryptGetProperty(alg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&objectLength, sizeof(objectLength), &numBytes, 0))) {
        throw runtime_error{ "Failed to get algorithm object length" };
    }

    if (!NT_SUCCESS(BCryptGetProperty(alg, BCRYPT_HASH_LENGTH, (PUCHAR)&hashLength, sizeof(hashLength), &numBytes, 0))) {
        throw runtime_error{ "Failed to get the hash length of the algorithm" };
    }

    // Create the hash object.
    BCRYPT_HASH_HANDLE hash{};
    vector<UCHAR> object{};

    object.resize(objectLength);

    if (!NT_SUCCESS(BCryptCreateHash(alg, &hash, object.data(), object.size(), nullptr, 0, 0))) {
        throw runtime_error{ "Failed to create hash object" };
    }

    SCOPE_EXIT([=] { BCryptDestroyHash(hash); });

    // Hash the string.
    if (!NT_SUCCESS(BCryptHashData(hash, (PUCHAR)data, length, 0))) {
        throw runtime_error{ "Failed to hash the input string" };
    }

    // Finalize the hash.
    vector<uint8_t> result{};

    result.resize(hashLength);

    if (!NT_SUCCESS(BCryptFinishHash(hash, (PUCHAR)result.data(), result.size(), 0))) {
        throw runtime_error{ "Failed to finalize the hash" };
    }

    return result;
}

vector<uint8_t> hashString(string_view algorithm, string_view str) {
    return hashData(algorithm, (const uint8_t*)str.data(), str.length());
}

vector<uint8_t> genRandomBuffer(size_t length) {
    vector<uint8_t> buffer{};

    buffer.resize(length);

    if (!NT_SUCCESS(BCryptGenRandom(nullptr, buffer.data(), buffer.size(), BCRYPT_USE_SYSTEM_PREFERRED_RNG))) {
        throw runtime_error{ "Failed to gen random buffer" };
    }

    return buffer;
}

vector<uint8_t> encryptData(string_view password, const uint8_t* data, size_t length) {
    // Create CNG algorithm provider.
    BCRYPT_ALG_HANDLE alg{};

    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&alg, BCRYPT_AES_ALGORITHM, nullptr, 0))) {
        throw runtime_error{ "Failed to open algorithm provider" };
    }

    SCOPE_EXIT([=] { BCryptCloseAlgorithmProvider(alg, 0); });

    // Set the chaining mode.
    if (!NT_SUCCESS(BCryptSetProperty(alg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0))) {
        throw runtime_error{ "Failed to set the algorithm chaining mode" };
    }

    // Get the size required for the key object and auth tag.
    ULONG numBytes{};
    ULONG objectLength{};
    BCRYPT_AUTH_TAG_LENGTHS_STRUCT authTagLengths{};

    if (!NT_SUCCESS(BCryptGetProperty(alg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&objectLength, sizeof(objectLength), &numBytes, 0))) {
        throw runtime_error{ "Failed to get algorithm object length" };
    }

    if (!NT_SUCCESS(BCryptGetProperty(alg, BCRYPT_AUTH_TAG_LENGTH, (PUCHAR)&authTagLengths, sizeof(authTagLengths), &numBytes, 0))) {
        throw runtime_error{ "Failed to get algorithm auth tag lengths" };
    }

    // Generate the key from the supplied password.
    BCRYPT_KEY_HANDLE key{};
    vector<uint8_t> object{};
    auto secret = hashString("SHA256", password);

    object.resize(objectLength);

    if (!NT_SUCCESS(BCryptGenerateSymmetricKey(alg, &key, object.data(), object.size(), secret.data(), secret.size(), 0))) {
        throw runtime_error{ "Failed to generate symmetric key object from password" };
    }

    SCOPE_EXIT([=] { BCryptDestroyKey(key); });

    // Encrypt it.
    vector<uint8_t> ciphertext{};
    vector<uint8_t> tag{};
    auto nonce = genRandomBuffer(12);
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo{};

    ciphertext.resize(length);
    tag.resize(authTagLengths.dwMinLength);
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);

    authInfo.pbNonce = nonce.data();
    authInfo.cbNonce = nonce.size();
    authInfo.pbAuthData = nonce.data();
    authInfo.cbAuthData = nonce.size();
    authInfo.pbTag = tag.data();
    authInfo.cbTag = tag.size();

    if (!NT_SUCCESS(BCryptEncrypt(
        key, 
        (PUCHAR)data, 
        length, 
        &authInfo, 
        0, 
        0, 
        ciphertext.data(), 
        ciphertext.size(), 
        &numBytes, 
        0
    ))) {
        throw runtime_error{ "Failed to encrypt input buffer" };
    }

    // Build the result buffer (nonce + tag + ciphertext).
    vector<uint8_t> result{};

    copy(nonce.begin(), nonce.end(), back_inserter(result));
    copy(tag.begin(), tag.end(), back_inserter(result));
    copy(ciphertext.begin(), ciphertext.end(), back_inserter(result));

    return result;
}

vector<uint8_t> encryptData(string_view password, const vector<uint8_t>& data) {
    return encryptData(password, data.data(), data.size());
}

vector<uint8_t> encryptString(string_view password, string_view str) {
    return encryptData(password, (const uint8_t*)str.data(), str.length() + 1);
}

vector<uint8_t> decryptData(string_view password, const uint8_t* data, size_t length) {
    // Create CNG algorithm provider.
    BCRYPT_ALG_HANDLE alg{};

    if (!NT_SUCCESS(BCryptOpenAlgorithmProvider(&alg, BCRYPT_AES_ALGORITHM, nullptr, 0))) {
        throw runtime_error{ "Failed to open algorithm provider" };
    }

    SCOPE_EXIT([=] { BCryptCloseAlgorithmProvider(alg, 0); });

    // Set the chaining mode.
    if (!NT_SUCCESS(BCryptSetProperty(alg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0))) {
        throw runtime_error{ "Failed to set the algorithm chaining mode" };
    }

    // Get the size required for the key object and auth tag.
    ULONG numBytes{};
    ULONG objectLength{};
    BCRYPT_AUTH_TAG_LENGTHS_STRUCT authTagLengths{};

    if (!NT_SUCCESS(BCryptGetProperty(alg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&objectLength, sizeof(objectLength), &numBytes, 0))) {
        throw runtime_error{ "Failed to get algorithm object length" };
    }

    if (!NT_SUCCESS(BCryptGetProperty(alg, BCRYPT_AUTH_TAG_LENGTH, (PUCHAR)&authTagLengths, sizeof(authTagLengths), &numBytes, 0))) {
        throw runtime_error{ "Failed to get algorithm auth tag lengths" };
    }

    // Extract the nonce, tag and cipher text from the supplied data.
    const auto headerLength = 12 + authTagLengths.dwMinLength;

    if (length < headerLength) {
        throw runtime_error{ "Supplied buffer is too small to decrypt" };
    }

    vector<uint8_t> nonce{};
    vector<uint8_t> tag{};
    vector<uint8_t> ciphertext{};

    copy_n(data, 12, back_inserter(nonce));
    copy_n(data + 12, authTagLengths.dwMinLength, back_inserter(tag));
    copy_n(data + headerLength, length - headerLength, back_inserter(ciphertext));

    // Generate the key from the supplied password.
    BCRYPT_KEY_HANDLE key{};
    vector<uint8_t> object{};
    auto secret = hashString("SHA256", password);

    object.resize(objectLength);

    if (!NT_SUCCESS(BCryptGenerateSymmetricKey(alg, &key, object.data(), object.size(), secret.data(), secret.size(), 0))) {
        throw runtime_error{ "Failed to generate symmetric key object from password" };
    }

    SCOPE_EXIT([=] { BCryptDestroyKey(key); });

    // Decrypt it.
    vector<uint8_t> plaintext{};
    vector<uint8_t> mac{};
    BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo{};

    plaintext.resize(ciphertext.size());
    mac.resize(authTagLengths.dwMaxLength);
    BCRYPT_INIT_AUTH_MODE_INFO(authInfo);

    authInfo.pbNonce = nonce.data();
    authInfo.cbNonce = nonce.size();
    authInfo.pbAuthData = nonce.data();
    authInfo.cbAuthData = nonce.size();
    authInfo.pbTag = tag.data();
    authInfo.cbTag = tag.size();
    authInfo.pbMacContext = mac.data();
    authInfo.cbMacContext = mac.size();

    if (!NT_SUCCESS(BCryptDecrypt(
        key, 
        ciphertext.data(), 
        ciphertext.size(), 
        &authInfo, 
        0, 
        0, 
        plaintext.data(), 
        plaintext.size(), 
        &numBytes, 
        0
    ))) {
        throw runtime_error{ "Failed to decrypt input buffer" };
    }

    return plaintext;
}

vector<uint8_t> decryptData(string_view password, const vector<uint8_t>& data) {
    return decryptData(password, data.data(), data.size());
}

string stringEncode(int encoding, const uint8_t* data, size_t length) {
    encoding |= CRYPT_STRING_NOCRLF;

    DWORD size{};

    if (CryptBinaryToString(data, length, encoding, nullptr, &size) == FALSE) {
        throw runtime_error{ "Failed to calculate a size for the encoded string" };
    }

    wstring result{};

    result.resize(size);

    if (CryptBinaryToString(data, length, encoding, result.data(), &size) == FALSE) {
        throw runtime_error{ "Failed to encode the data into a string" };
    }

    return narrow(result.substr(0, result.length() - 1));
}

string stringEncode(int encoding, const vector<uint8_t>& data) {
    return stringEncode(encoding, data.data(), data.size());
}

