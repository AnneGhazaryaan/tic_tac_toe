#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QObject>
#include <QString>

class CryptoManager : public QObject
{
    Q_OBJECT

public:
    explicit CryptoManager(QObject *parent = nullptr);
    QString aesEncrypt(const QByteArray &data, const QByteArray &key);
    QString aesDecrypt(const QByteArray &cipherText, const QByteArray &key);

private:
    QByteArray aesEncryptBytes(const QByteArray &data, const QByteArray &key);
    QByteArray aesDecryptBytes(const QByteArray &cipherText, const QByteArray &key);
    QByteArray generateAesKey(const QString &key);
};

#endif // CRYPTOMANAGER_H
