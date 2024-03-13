#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QPushButton>

class GameLogic : public QObject {
    Q_OBJECT
public:
    explicit GameLogic(QObject *parent = nullptr);
    bool checkLine(QPushButton* button1, QPushButton* button2, QPushButton* button3) const;
    QString updateGameState(QPushButton* buttons[], const QString& currentGameState);
    bool allButtonsSet(QPushButton* buttons[]);
};

#endif // GAMELOGIC_H
