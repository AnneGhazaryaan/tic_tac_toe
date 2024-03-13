#include "cryptomanager.h"
#include <QCryptographicHash>
#include <openssl/aes.h>
#include <openssl/rand.h>

CryptoManager::CryptoManager(QObject *parent) : QObject(parent) {}

QString CryptoManager::aesEncrypt(const QByteArray &data, const QByteArray &key)
{
    QByteArray result;

    // Ensure the key is of valid size
    QByteArray keyResized = key.left(AES_BLOCK_SIZE);
    if (keyResized.size() < AES_BLOCK_SIZE) {
        keyResized.append(QByteArray(AES_BLOCK_SIZE - keyResized.size(), '\0'));
    }

    // Prepare encryption key
    AES_KEY aesKey;
    if (AES_set_encrypt_key((const unsigned char*)keyResized.constData(), 128, &aesKey) < 0) {
        return QString(); // Return empty QString on error
    }

    // Input data needs to be a multiple of AES_BLOCK_SIZE
    QByteArray inputData = data;
    int paddingLength = AES_BLOCK_SIZE - (data.size() % AES_BLOCK_SIZE);
    inputData.append(QByteArray(paddingLength, paddingLength));

    // Encrypt data
    const size_t inputSize = inputData.size();
    unsigned char* encrypted = new unsigned char[inputSize];
    for (unsigned int i = 0; i < inputSize; i += AES_BLOCK_SIZE) {
        AES_encrypt((const unsigned char*)inputData.constData() + i, encrypted + i, &aesKey);
    }

    result = QByteArray(reinterpret_cast<char*>(encrypted), inputSize);
    delete[] encrypted;

    return QString(result.toBase64()); // Encode to Base64 and convert to QString
}

QString CryptoManager::aesDecrypt(const QByteArray &cipherText, const QByteArray &key)
{
    QByteArray result;
    QByteArray decodedData = QByteArray::fromBase64(cipherText);

    // Ensure the key is of valid size
    QByteArray keyResized = key.left(AES_BLOCK_SIZE);
    if (keyResized.size() < AES_BLOCK_SIZE) {
        keyResized.append(QByteArray(AES_BLOCK_SIZE - keyResized.size(), '\0'));
    }

    // Prepare decryption key
    AES_KEY aesKey;
    if (AES_set_decrypt_key((const unsigned char*)keyResized.constData(), 128, &aesKey) < 0) {
        return QString(); // Return empty QString on error
    }

    // Decrypt data
    const size_t inputSize = decodedData.size();
    unsigned char* decrypted = new unsigned char[inputSize];
    for (unsigned int i = 0; i < inputSize; i += AES_BLOCK_SIZE) {
        AES_decrypt((const unsigned char*)decodedData.constData() + i, decrypted + i, &aesKey);
    }

    result = QByteArray(reinterpret_cast<char*>(decrypted), inputSize);

    // Remove padding
    int padLength = result.at(result.size() - 1);
    result.chop(padLength);

    delete[] decrypted;

    return QString(result); // Convert decrypted result to QString
}
