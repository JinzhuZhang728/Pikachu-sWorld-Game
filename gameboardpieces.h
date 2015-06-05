#ifndef GAMEBOARDPIECES_H
#define GAMEBOARDPIECES_H

#include <QLabel>

namespace Ui {
class GameBoardPieces;
}

class GameBoardPieces : public QLabel
{
    Q_OBJECT

    friend class GameBoard;
public:
    //default GameBoardPieces contructor
    explicit GameBoardPieces(QWidget* parent =0, size_t position = 0, bool is_wall = 0, bool is_coin = 0, bool is_diamond = 0, bool is_cherry = 0);

    ~GameBoardPieces();

private:
    Ui::GameBoardPieces *ui;
    QPixmap* wall_image;
    QPixmap* coin_image;
    QPixmap* diamond_image;
    QPixmap* cherry_image;

    size_t position;
    bool is_wall;
    bool is_coin;
    bool is_diamond;
    bool is_cherry;
    bool bomb;
};

#endif // GAMEBOARDPIECES_H
