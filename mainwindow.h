#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include "gameboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void easy_game_begin();
    void medium_game_begin();
    void hard_game_begin();
    void how_to_play();

    void game_over();
    void quit();
    //void restart();


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameBoard* board;

    //QMessageBox* messageBox;
};

#endif // MAINWINDOW_H
