#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include "cryptomanager.h"

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    void connectToServer(const QString& host, quint16 port);
    void sendMessage(const QString& message);

signals:
    void dataReceived(const QString& data);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
    CryptoManager *cryptoManager;
};

#endif // NETWORKMANAGER_H
