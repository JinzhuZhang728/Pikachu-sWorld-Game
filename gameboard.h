#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QMainWindow>
#include <QKeyEvent>

#include<QMouseEvent>
#include<sstream>
#include<string>
#include<QSound>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <chrono>
#include <QTimer>
#include <random>
#include "gameboardpieces.h"

namespace Ui {
class GameBoard;
}

class GameBoard : public QWidget
{
    Q_OBJECT

public slots:
    void update_monsters();


public:
    explicit GameBoard(QWidget *parent = 0, size_t num_coins = 0,size_t num_diamonds = 0,size_t num_cherries = 0, size_t board_size = 10, size_t number_monster = 4, size_t monster_speed = 10);
    void create_wall();
    void create_coin();
    void create_diamond();
    void create_cherry();

    void eaten();

    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

    void updatePlayer(int px, int py, int nx, int ny);
    ~GameBoard();

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::GameBoard *ui;

    size_t num_coins;
    size_t num_diamonds;
    size_t num_cherries;
    size_t coins_collected=0;

    //Top Bar Variables
    QWidget *Top;
    QLabel* game_type_value;
    unsigned int lives_remaining;
    QLabel** lives;
    unsigned int bombs_remaining;
    QLabel** bombs;
    bool drop_enabled;

    QTimer *timer;

    // Board Variables
    QWidget *Board = new QWidget;
    QGridLayout *SquareGrid = new QGridLayout(Board);
    size_t board_size;
    int label_size = 480/board_size;
    size_t monster_speed;
    GameBoardPieces** labels;

    QPoint *player_position;
    QPoint *monster_positions;
    size_t number_monster;
    QPoint bomb_drop_pos;
    QPoint selected_bomb_pos;

    const QPixmap* player_image;
    QPixmap* monster_image;
    QPixmap* bomb_image;

    std::default_random_engine generator;

};

#endif // GAMEBOARD_H
