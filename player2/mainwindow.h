#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "networkmanager.h"
#include "gamelogic.h"
#include "cryptomanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonClicked();
    void readData(const QString& data);
    void announce(const QString& gameState);
    bool areAllButtonsEnabled();

private:
    Ui::MainWindow *ui;
    NetworkManager *networkManager;
    GameLogic *gameLogic;
    void disableAllButtons();
    void enableAllButtons();
    QPushButton* buttons[9];
    QString gameStatus;
};

#endif // MAINWINDOW_H
