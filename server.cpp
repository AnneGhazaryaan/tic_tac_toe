#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>  
#include <QDebug>

QTcpSocket* player1Socket = nullptr;
QTcpSocket* player2Socket = nullptr;

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("game_data.db");
    if (!db.open()) {
        qDebug() << "Error: Unable to open the database.";
        return false;
    }
    return true;
}

bool createTable() {
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS game_history ("
                    "player_id INTEGER, "
                    "game_state TEXT, "
                    "game_result TEXT DEFAULT NULL)")) {
        qDebug() << "Error: Failed to create table." << query.lastError();
        return false;
    }
    return true;
}

void forwardMessage(QTcpSocket* sender, const QByteArray& message, QTcpSocket* receiver) {
    qDebug() << "Forwarding message: " << message;

    int player_id = (sender == player1Socket) ? 1 : 2;

    QSqlQuery query;
    query.prepare("INSERT INTO game_history (player_id, game_state) VALUES (:player_id, :game_state)");
    query.bindValue(":player_id", player_id);
    query.bindValue(":game_state", QString(message));

    if (!query.exec()) {
        qDebug() << "Error inserting data into the database: " << query.lastError();
    }

    if (receiver && receiver->isWritable()) {
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

        QTcpSocket* sender = (clientSocket == player1Socket) ? player1Socket : player2Socket;
        QTcpSocket* receiver = (clientSocket == player1Socket) ? player2Socket : player1Socket;

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

    if (!initializeDatabase() || !createTable()) {
        return 1; // Exit if database setup fails
    }

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
