#include "gameboardpieces.h"
#include "ui_gameboardpieces.h"
/**
 * @class GameBoardPieces
 * @brief this class constructs the pieces that made up the gameboard
 * @details each piece has its own specialized property, whether it's a blank piece, or a coin, or a diamond, or a cherry
*/
/**
 * @brief GameBoardPieces::GameBoardPieces constructor
 * @param parent sets the parent widget to be the board widget
 * @param position is the position of the piece relative to the board
 * @param is_wall whether the piece is a wall
 * @param is_coin whether the piece is a coin
 * @param is_diamond whether the piece is a diamond
 * @param is_cherry whether the piece is a cherry
 */
GameBoardPieces::GameBoardPieces(QWidget* parent, size_t position, bool is_wall, bool is_coin, bool is_diamond, bool is_cherry) :
     QLabel(parent), position(position), is_wall(is_wall), is_coin(is_coin), is_diamond(is_diamond), is_cherry(is_cherry),
    ui(new Ui::GameBoardPieces)
{
    ui->setupUi(this);
    this->setFrameStyle(3);
    this->setAlignment(Qt::AlignCenter);
    bomb = false;
    //if this piece is a wall, set the image to be a wall image
    if(is_wall){
        QString fileName(":/wall_image.bmp");
        wall_image = new QPixmap(fileName);
        this->setPixmap(*wall_image);
        this->setScaledContents(true);
    }
    //if this piece is a coin, set the image to be a coin image
    else if(is_coin){
        QString fileName(":/coin_image.bmp");
        coin_image = new QPixmap(fileName);
        this->setPixmap(*coin_image);
        this->setScaledContents(true);
    }
    //if this piece is a diamond, set the image to be a diamond image
    else if(is_diamond){
        QString fileName(":/diamond_image.bmp");
        diamond_image = new QPixmap(fileName);
        this->setPixmap(*diamond_image);
        this->setScaledContents(true);
    }
    //if this piece is a cherry, set the image to be a cherry image
    else if(is_cherry){
        QString fileName(":/cherry_image.bmp");
        cherry_image = new QPixmap(fileName);
        this->setPixmap(*cherry_image);
        this->setScaledContents(true);
    }
    else
        this->setStyleSheet("QLabel { background-color : black; color: black;}");
}

GameBoardPieces::~GameBoardPieces()
{
    delete ui;
}

