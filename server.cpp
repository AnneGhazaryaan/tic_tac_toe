#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>

QTcpSocket* player1Socket = nullptr;
QTcpSocket* player2Socket = nullptr;

void forwardMessage(QTcpSocket* sender, const QByteArray& message, QTcpSocket* receiver) {
    qDebug() << "Forwarding message: " << message;

    if (receiver && receiver->isWritable()) {
        qDebug() << "Receiver is writable, writing message to receiver";
        receiver->write(message);
        receiver->flush();
    } else {
        qDebug() << "Unable to forward message to the receiver.";
    }
}

void handlePlayerConnection(QTcpSocket* clientSocket) {
    qDebug() << "New connection established.";

    if (!player1Socket) {
        player1Socket = clientSocket;
        qDebug() << "Player 1 connected.";
    } else if (!player2Socket) {
        player2Socket = clientSocket;
        qDebug() << "Player 2 connected.";
    }

    QObject::connect(clientSocket, &QTcpSocket::readyRead, [clientSocket]() {
        QByteArray data = clientSocket->readAll();
        qDebug() << "Received data from a player: " << data;

        // Identify the sender and receiver
        QTcpSocket* sender = nullptr;
        QTcpSocket* receiver = nullptr;

        if (clientSocket == player1Socket) {
            sender = player1Socket;
            receiver = player2Socket;
        } else if (clientSocket == player2Socket) {
            sender = player2Socket;
            receiver = player1Socket;
        }

        // Forward the message to the other player
        forwardMessage(sender, data, receiver);
    });

    QObject::connect(clientSocket, &QTcpSocket::disconnected, [clientSocket]() {
        qDebug() << "Player disconnected.";
        if (clientSocket == player1Socket) {
            player1Socket = nullptr;
        } else if (clientSocket == player2Socket) {
            player2Socket = nullptr;
        }
        clientSocket->deleteLater();
    });
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QTcpServer server;
    if (!server.listen(QHostAddress::Any, 1234)) {
        qDebug() << "Error: Unable to start the server. " << server.errorString();
        return 1;
    }

    qDebug() << "Server listening on port 1234...";

    QObject::connect(&server, &QTcpServer::newConnection, [&server]() {
        QTcpSocket* clientSocket = server.nextPendingConnection();
        handlePlayerConnection(clientSocket);
    });

    return a.exec();
}
