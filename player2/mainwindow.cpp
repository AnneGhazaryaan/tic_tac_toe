#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    networkManager(new NetworkManager(this)),
    gameLogic(new GameLogic(this)),
    gameStatus("----")
{
    ui->setupUi(this);
    setFixedSize(400, 400); // Keep the window size fixed
    setWindowTitle("Player2");

    // Setup buttons array for easy access
    buttons[0] = ui->button1;
    buttons[1] = ui->button2;
    buttons[2] = ui->button3;
    buttons[3] = ui->button4;
    buttons[4] = ui->button5;
    buttons[5] = ui->button6;
    buttons[6] = ui->button7;
    buttons[7] = ui->button8;
    buttons[8] = ui->button9;

    int buttonSize = 60;
    for (QPushButton* button : buttons) {
        button->setFixedSize(buttonSize, buttonSize);
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        button->setStyleSheet("background-color: #FFD1DC");
    }

    // Connect buttons to a single slot
    for(QPushButton* button : buttons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::on_buttonClicked);
    }

    connect(networkManager, &NetworkManager::dataReceived, this, &MainWindow::readData);
    networkManager->connectToServer("localhost", 1234);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button && button->text().isEmpty()) {
        button->setText("O"); // Player 2's move
        gameStatus = gameLogic->updateGameState(buttons, gameStatus);
        networkManager->sendMessage(gameStatus + "@" + button->objectName());
        disableAllButtons(); // Disable buttons to wait for the opponent's move
        announce(gameStatus);
        button->setStyleSheet("background-color: #D3D3D3");
    }
}

void MainWindow::readData(const QString& data) {
    QStringList parts = data.split('@');

    QString receivedStatus = parts.at(0);
    QString buttonName = parts.at(1);

    for(QPushButton* button : buttons) {
        if(button->objectName() == buttonName) {
            button->setText("X");
            button->setStyleSheet("background-color: #D3D3D3");
            break;
        }
    }

    enableAllButtons();
    announce(receivedStatus);
}

void MainWindow::announce(const QString& gameState)
{
    if (gameState == "1w2l") {
        qDebug() << "Player 2 lose!";
        QMessageBox::critical(this, "Player 2","You lose!");
        disableAllButtons();
    } else if (gameState == "1l2l") {
        qDebug() << "It's a draw!";
        QMessageBox::about(this, "Player 2","It's a draw!");
        disableAllButtons();
    } else if (gameState == "1l2w") {
        qDebug() << "Player 2 wins!";
        QMessageBox::information(this, "Player 2","You win!");
        disableAllButtons();
    }
}

bool MainWindow::areAllButtonsEnabled() {
    return ui->button1->isEnabled() && ui->button2->isEnabled() &&
           ui->button3->isEnabled() && ui->button4->isEnabled() &&
           ui->button5->isEnabled() && ui->button6->isEnabled() &&
           ui->button7->isEnabled() && ui->button8->isEnabled() &&
           ui->button9->isEnabled();
}

void MainWindow::disableAllButtons()
{
    for(QPushButton* button : buttons) {
        button->setEnabled(false);
        button->setStyleSheet("background-color: #D3D3D3");
    }
}

void MainWindow::enableAllButtons()
{
    for(QPushButton* button : buttons) {
        if(button->text().isEmpty()) { // Only re-enable buttons that haven't been clicked
            button->setEnabled(true);
            button->setStyleSheet("background-color: #FFD1DC");
        } else {
            button->setStyleSheet("background-color: #D3D3D3");
        }
    }
}
