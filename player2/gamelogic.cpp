#include "gamelogic.h"

GameLogic::GameLogic(QObject *parent) : QObject(parent) {}

bool GameLogic::checkLine(QPushButton* button1, QPushButton* button2, QPushButton* button3) const {
    return button1->text() == button2->text() && button2->text() == button3->text() && !button1->text().isEmpty();
}

QString GameLogic::updateGameState(QPushButton* buttons[], const QString& currentGameState)
{
    QString gameState = currentGameState;

    // Check horizontal lines
    if (checkLine(buttons[0], buttons[1], buttons[2])) {
        gameState = "1l2w";
    } else if (checkLine(buttons[3], buttons[4], buttons[5])) {
        gameState = "1l2w";
    } else if (checkLine(buttons[6], buttons[7], buttons[8])) {
        gameState = "1l2w";
    }
    // Check vertical lines
    else if (checkLine(buttons[0], buttons[3], buttons[6])) {
        gameState = "1l2w";
    } else if (checkLine(buttons[1], buttons[4], buttons[7])) {
        gameState = "1l2w";
    } else if (checkLine(buttons[2], buttons[5], buttons[8])) {
        gameState = "1l2w";
    }
    // Check diagonal lines
    else if (checkLine(buttons[0], buttons[4], buttons[8])) {
        gameState = "1l2w";
    } else if (checkLine(buttons[2], buttons[4], buttons[6])) {
        gameState = "1l2w";
    }
    // If no winner, check for a draw
    else if (allButtonsSet(buttons)) {
        gameState = "1l2l";
    }
    // If none of the above, the game continues
    else {
        gameState = "----";
    }

    return gameState;
}

bool GameLogic::allButtonsSet(QPushButton* buttons[])
{
    for (int i = 0; i < 9; ++i) {
        if (buttons[i]->text().isEmpty()) {
            return false; // At least one button is not set
        }
    }
    return true; // All buttons are set
}
