#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.h"
#include <QLabel>
#include <QMessageBox>
/** @mainpage PIC 10C Spring 2015 QT Project
    @author Jinzhu Zhang
    @date June 4th, 2015
    @brief This is the documentation for code in my QT final project
*/
/**
 * @class MainWindow
 * @brief this class constructs the mainwindow(mainpage)
*/
/**
 * @brief MainWindow constructor
 * @details constructs the mainpage of the game, which allows players to choose different difficulty levels
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QPixmap bkgnd(":/pikachu_background_2.bmp");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}
/**
 * @brief constructs the GameBoard of the easy level
 */
void MainWindow::easy_game_begin() {
    board = new GameBoard(this, 15, 2, 3, 12, 4, 100);
    this->setCentralWidget(board);

}
/**
 * @brief constructs the GameBoard of the medium level
 */
void MainWindow::medium_game_begin() {
    board = new GameBoard(this,25, 3, 3, 16, 6, 50);
    this->setCentralWidget(board);
}
/**
 * @brief constructs the GameBoard of the hard level
 */
void MainWindow::hard_game_begin() {
    board = new GameBoard(this,40, 4, 3, 20, 8, 10);
    this->setCentralWidget(board);
}
/**
 * @brief constructs a window that shows the player the game instructions
 */
void MainWindow::how_to_play(){
    QWidget* instr_window = new QWidget;
        //divide the game instructions into 4 paragraphs, each paragraph is a seperate label, put the labels in a vertical layout
    QLayout* instr_layout = new QVBoxLayout(instr_window);
    QLabel* instructions1 = new QLabel("Player can use the arrow keys to control the Pikachu to move around to collect coins and diamonds.");
    instructions1->setContentsMargins(10,10,10,10);
    instructions1->setWordWrap(true);
    QLabel* instructions2 = new QLabel("One diamond is equivalent with 10 coins.When the Pikachu has collected all coins available, the player wins. ");
    instructions2->setContentsMargins(10,10,10,10);
    instructions2->setWordWrap(true);
    QLabel* instructions3 = new QLabel("When the Pikachu collides a little monster, it looses a life, "
                                      "but the game continues as long as there’re still lives remaining. It has four lives in total. "
                                      "When the player runs out of life, the game is over. "
                                       "Be Careful! Little monsters may hide behind the coins, diamonds, and the walls!");
    instructions3->setContentsMargins(10,10,10,10);
    instructions3->setWordWrap(true);
    QLabel* instructions4 = new QLabel("When the Pikachu eats a cherry, the player gets a bomb, which will appear on the top bar. "
                                       "The player can pick up the bomb by clicking left button, "
                                       "and drop it to anywhere on the board by clicking right button. "
                                       "If the little monster steps on the bomb, it dies or freezes.");
    instructions4->setContentsMargins(10,10,10,10);
    instructions4->setWordWrap(true);
    instr_layout->addWidget(instructions1);
    instr_layout->addWidget(instructions2);
    instr_layout->addWidget(instructions3);
    instr_layout->addWidget(instructions4);
    instr_window->show();
    instr_window->setFixedSize(500, 400);
    //sets up the background picture for the instruction window
    QPixmap bkgnd(":/pikachu_background.bmp");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    instr_window->setPalette(palette);
}
/**
 * @brief exit the game gracefully and return to the mainpage
 */
void MainWindow::game_over(){
    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    //delete wid;
    wid->setParent(nullptr);
    ui->setupUi(this);

}

void MainWindow::quit(){
    game_over();

}


MainWindow::~MainWindow()
{
    delete ui;
}

