#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    cryptoManager = new CryptoManager(this); // Initialize CryptoManager
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
}

void NetworkManager::connectToServer(const QString& host, quint16 port) {
    qDebug() << "Attempting to connect to server...";
    socket->connectToHost(host, port);
    if(socket->waitForConnected(5000)) {
        qDebug() << "Connected to server.";
    } else {
        qDebug() << "Failed to connect to server.";
    }
}

void NetworkManager::sendMessage(const QString& message) {
    if(socket->isWritable()) {
        QByteArray messageData = message.toUtf8(); // Convert QString to QByteArray
        QString encryptedMessage = cryptoManager->aesEncrypt(messageData, "your_secret_key");
        socket->write(encryptedMessage.toUtf8());
        socket->flush();
        qDebug() << "Encrypted message sent to server: " << encryptedMessage;
    }
}

void NetworkManager::onReadyRead() {
    QByteArray data = socket->readAll();
    QString decryptedMessage = cryptoManager->aesDecrypt(data, "your_secret_key"); // Pass QByteArray directly
    qDebug() << "Decrypted data received from server: " << decryptedMessage;
    emit dataReceived(decryptedMessage); // Ensure to emit the signal with decrypted data
}
