#include "gameboard.h"
#include "ui_gameboard.h"
#include <qpushbutton.h>
#include <ctime>
#include <QMessageBox>
#include <iostream>
#include <set>
/**
 * @class GameBoard
 * @brief this class constructs the actual game window
*/
/**
 * @brief GameBoard constructor
 * @param parent sets up the parent widget of the GameBoard widget
 * @param num_coins is the number of coins available on the board
 * @param num_diamonds is the number of diamonds available on the board
 * @param num_cherries is the number of cherries available on the board
 * @param board_sz is the size of the board
 * @param number_monster is the number of monsters that will be walking around
 * @param monster_spd sets up the walking speed of the monsters
 */
GameBoard::GameBoard(QWidget* parent, size_t num_coins,size_t num_diamonds,size_t num_cherries, size_t board_sz, size_t number_monster, size_t monster_spd) :
     QWidget(parent),
    ui(new Ui::GameBoard), num_coins(num_coins),num_diamonds(num_diamonds),num_cherries(num_cherries), board_size(board_sz), number_monster(number_monster), monster_speed(monster_spd)
{
    ui->setupUi(this);

    unsigned new_seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(new_seed);


    Top = new QWidget;
    Top->setStyleSheet("QLabel { background-color : black; color : white; }");
    // This code sets up the Top bar

    // P P P B B B      coins collected: #

    // This layout will space out the elements above
    QHBoxLayout *top_bar = new QHBoxLayout(Top);

    // Load up the image of the pikachus and mosnters!

    QString monster_filename(":/monster_image.bmp");
    monster_image = new QPixmap(monster_filename);
    QString PlayerfileName(":/player_image.bmp");
    player_image = new QPixmap(PlayerfileName);

    // Fix a total of 3 lives for now, can change later.
    // M M M
    // One life is currently playing, the others are listed above
    lives_remaining = 4;
    lives = new QLabel*[lives_remaining-1];
    // This line above an array of QLabel POINTERS.

    // Set the labels to have the pikachu image, add to the top bar layout.
    for(size_t i=0;i<lives_remaining-1;i++) {
        lives[i] = new QLabel;
        lives[i]->setPixmap(*player_image);
        lives[i]->setMinimumSize(40,40);
        lives[i]->setMaximumSize(40,40);
        lives[i]->setScaledContents(true);
        top_bar->addWidget(lives[i]);
    }
    //set up the bomb image
    QString bomb_filename(":/bomb_image.bmp");
    bomb_image = new QPixmap(bomb_filename);

    //there will be a total of 3 bombs available for the player to gain
    size_t total_bombs = 3;
    //bombs_remaining indicates how many bombs the player still has
    bombs_remaining = 0;

    bombs = new QLabel*[total_bombs];

    //add the bombs images to the top bar, set them to hide mode, when the player eats a cherry, one bomb will show
    for(size_t i=0;i<total_bombs;i++) {
        bombs[i] = new QLabel;
        bombs[i]->setPixmap(*bomb_image);
        bombs[i]->setMinimumSize(40,40);
        bombs[i]->setMaximumSize(40,40);
        bombs[i]->setScaledContents(true);
        top_bar->addWidget(bombs[i]);
    }
    bombs[0]->hide();
    bombs[1]->hide();
    bombs[2]->hide();


    // Add in some horizontal space before next labels
    QSpacerItem* horizontal_space = new QSpacerItem(10,10);

    top_bar->setSpacing(0);

    // P P P B B B     Coins Collected: #
    QLabel* game_type_text = new QLabel("Coins Collected: ");
    game_type_value = new QLabel(QString::number(coins_collected));
    game_type_text->setFixedHeight(40);
    game_type_value->setFixedHeight(40);

    top_bar->addWidget(game_type_text);
    top_bar->addWidget(game_type_value);

    // This code Creates the Board
    labels = new GameBoardPieces*[board_size*board_size];
    //set up the Board layout to be grid layout
    SquareGrid->setGeometry(QRect());
    SquareGrid->setSpacing(0);

    // Create each label for the board, initialize with plain GameBoardPieces.
    for(size_t i=0;i<board_size;i++) {
        for(size_t j=0;j<board_size;j++) {

            // Create label and set properties.
            labels[i*board_size+j] = new GameBoardPieces(Board, i*board_size+j, 0, 0, 0, 0);
            labels[i*board_size+j]->setMinimumSize(label_size,label_size);
            labels[i*board_size+j]->setMaximumSize(label_size,label_size);
            // Add label to the layout
            SquareGrid -> addWidget(labels[i*board_size+j] ,i,j);

        }
    }

    //create features of the board, including walls, coins, diamonds, and cherries
    srand(time(NULL));
    create_wall();
    create_coin();
    create_diamond();
    create_cherry();

    // Initialize the player at the top left corner, coordinate (0,0).
    player_position = new QPoint(0,0);
    labels[0]->setPixmap(*player_image);
    labels[0]->setScaledContents(true);

    //create an array of QPoint pointers pointing to the monster positions
    monster_positions = new QPoint[number_monster];

    // Set monster randomly on the board.

    for(size_t i = 0; i<number_monster; ++i){
        monster_positions[i].setX(rand()%board_size);
        monster_positions[i].setY(rand()%board_size);
    }
    //set up a QTimer, each time the timer goes out, it calls the update_monsters() function
    //the length of the timer is initalized differently according to the monster speed
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_monsters()));
    timer->start(monster_speed);

    //fix board size
    Board->setFixedSize(480,480);
    setFixedSize(500, 600);

    QVBoxLayout* GameWindow = new QVBoxLayout;

    //creates a quit button just below the board
    //when the player clicks the quit button, it returns to the mainpage
    QPushButton* quit = new QPushButton("Quit");
    QObject::connect(quit, SIGNAL(clicked()), parent, SLOT(quit()));

    GameWindow->addWidget(Top);
    GameWindow->addWidget(Board);
    GameWindow->addWidget(quit);
    this->setLayout(GameWindow);

}
/**
 * @brief GameBoard destructor
 */
GameBoard::~GameBoard()
{
    delete [] monster_positions;
    delete [] player_position;

    delete ui;
}
/**
 * @brief GameBoard::keyPressEvent
 * @details this event handler receives key press events for the widget
 * @param event is the key press events, in other words, when the player presses the arrow keys
 */
void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = player_position->rx();
    int y = player_position->ry();

    switch (event->key()) {
    case Qt::Key_Left:
        //the player can't go there if it's outside the board, or a wall, or a bomb
        if(player_position->rx() != 0 && !(labels[y*board_size+(x-1)]->is_wall) && !(labels[y*board_size+(x-1)]->bomb))
            updatePlayer(x,y,x-1,y);

        break;
    case Qt::Key_Right:
        if(player_position->rx() != board_size-1 && !(labels[y*board_size+(x+1)]->is_wall)&& !(labels[y*board_size+(x+1)]->bomb))
            updatePlayer(x,y,x+1,y);

        break;
    case Qt::Key_Up:
        if(player_position->ry() != 0 && !(labels[(y-1)*board_size+x]->is_wall)&& !(labels[(y-1)*board_size+x]->bomb))
            updatePlayer(x,y,x,y-1);

        break;
    case Qt::Key_Down:
        if(player_position->ry() != board_size-1 && !(labels[(y+1)*board_size+x]->is_wall)&& !(labels[(y+1)*board_size+x]->bomb))
            updatePlayer(x,y,x,y+1);

        break;

    default:
        QWidget::keyPressEvent(event);
    }

    return;
}
/**
 * @brief GameBoard::paintEvent is a request to repaint all or part of the widget, get called every time the player or the mosnter moves
 * @param e is the QPaintEvent
 */
void GameBoard::paintEvent(QPaintEvent *e) {

    int x = player_position->rx();
    int y = player_position->ry();

    labels[y*board_size+x]->setPixmap(*player_image);
    labels[y*board_size+x]->setScaledContents(true);


    for(size_t i=0;i<number_monster;i++) {
        int px = monster_positions[i].rx();
        int py = monster_positions[i].ry();

        //the monster can go to the labels that have been occupied by walls, coins, diamonds or cherries, but the monsters won't be shown
        if(px>=0 && py >= 0 && px<board_size && py<board_size && (!(labels[py*board_size+px]->is_wall)) && (!(labels[py*board_size+px]->is_coin))
            && (!(labels[py*board_size+px]->is_cherry)) && (!(labels[py*board_size+px]->is_diamond))) {
            labels[py*board_size+px]->setPixmap(*monster_image);
            labels[py*board_size+px]->setScaledContents(true);
        }
        //if the monster goes to a label with a bomb, it dies
        if(labels[py*board_size+px]->bomb){
            //move the dead monster out of the board
            monster_positions[i].setX(-1);
            //number of monsters is decreased by 1
            number_monster--;
            labels[py*board_size+px]->clear();
            labels[py*board_size+px]->setStyleSheet("QLabel { background-color : black; color: black;}");
            labels[py*board_size+px]->bomb = false;
            QSound::play(":/bomb_sound.wav");
        }
    }
    //update the coins collected
    game_type_value -> setText(QString::number(coins_collected));

}
/**
 * @brief GameBoard::showEvent
 * @param e is the QShowEvent
 */
void GameBoard::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}
/**
 * @brief GameBoard::mousePressEvent this event handles all requests sent by the mouse clicks
 * @param event is the QMouseEvent that's received every time the player clicks the mouse buttons
 */
void GameBoard::mousePressEvent(QMouseEvent *event){
    //when the player clicks left button on the bomb in the top bar, the bomb disappears from the bard and is ready to be dropped onto the board
    if(event->button() == Qt::LeftButton){
        selected_bomb_pos = mapToParent(event->pos());
        int i = (selected_bomb_pos.x()-10)/40;
        if((i-3)>=0 && (i-3)<=2)
            bombs[i-3]->hide();
        drop_enabled = true;
    }
    //when the player clicks right button anywhere on the board, the bomb wil be dropped at the particular position the cursor is at
    if(event->button() == Qt::RightButton && drop_enabled){
        if(bombs_remaining>=1){
            bomb_drop_pos = mapFromParent(event->pos());
            int x = (bomb_drop_pos.x()-10)/label_size;
            int y = (bomb_drop_pos.y()-80)/label_size;
            labels[y*board_size+x]->setPixmap(*bomb_image);
            labels[y*board_size+x]->setScaledContents(true);
            labels[y*board_size+x]->bomb = true;
            bombs_remaining--;
            drop_enabled = false;
        }
    }
}

/**
 * @brief GameBoard::updatePlayer
 * @details updates player's old position to new position according to the parameters, also updates the coins collected and bombs collected
 * @param px is the current x position of the player
 * @param py is the current y position of the player
 * @param nx is the next x position of the player
 * @param ny is the next y position of the player
 */
void GameBoard::updatePlayer(int px, int py, int nx, int ny) {
    labels[py*board_size+px]->clear();
    labels[py*board_size+px]->setStyleSheet("QLabel { background-color : black; color: black;}");

    player_position->setX(nx);
    player_position->setY(ny);

    //when the label at the next position has a coin, coins collected is increased by 1
    if(labels[ny*board_size+nx]->is_coin){
        coins_collected++;
        QSound::play(":/CollectCoin.wav");
        labels[ny*board_size+nx]->is_coin = 0;
    }
    //when the label at the next position has a diamond, coins collected is increased by 10
    if(labels[ny*board_size+nx]->is_diamond){
        coins_collected+=10;
        QSound::play(":/CollectCoin.wav");
        labels[ny*board_size+nx]->is_diamond = 0;
    }
    //when the label at the next position has a cherry, bombs_remaining is increased by 1 and it will shown at the top bar
    if(labels[ny*board_size+nx]->is_cherry){
        bombs_remaining++;
        bombs[bombs_remaining-1]->show();
        QSound::play(":/CollectCoin.wav");
        labels[ny*board_size+nx]->is_cherry = 0;
    }
    //when coins collected reaches the maximum availablle on board, the player wins and a window will pop up
    if(coins_collected == (num_coins+10*num_diamonds)){
        QVBoxLayout* winningLayout = new QVBoxLayout;
        QLabel* WinningMessage = new QLabel("Congratulations! You have passed this level!");
        QPushButton* play_again = new QPushButton("Play Again");
        QObject::connect(play_again, SIGNAL(clicked()), this->parent(), SLOT(quit()));
        winningLayout->addWidget(WinningMessage);
        winningLayout->addWidget(play_again);
        QWidget* Winning = new QWidget;
        Winning->setLayout(winningLayout);
        Winning->show();
    }

    eaten();

}
/**
 * @brief GameBoard::eaten
 * @details this funcion updates what's gonna happen if the pikachu collides with a little monster, the player loses a life and a window will pop up indicating the loss
 */
void GameBoard::eaten(){
    for(size_t i=0;i<number_monster;i++){
        if(player_position->rx() == monster_positions[i].rx() && player_position->ry() == monster_positions[i].ry()){
            if(lives_remaining > 1) {
                QLabel* life_loss = new QLabel("You have lost a life because you're eaten by the monster!");
                life_loss->setContentsMargins(10,10,10,10);
                life_loss->show();
                lives_remaining--;
                lives[lives_remaining-1]->clear();
            }
            //when the player runs out of life, the game is over, and a window pops up indicating it, and a restart button will be available
            else {
                QVBoxLayout* GameOverLayout = new QVBoxLayout;
                QLabel* game_over = new QLabel("Game Over!");
                QLabel* reason = new QLabel("You have run out of life!");
                QPushButton* restart = new QPushButton("Restart");
                QObject::connect(restart, SIGNAL(clicked()), this->parent(), SLOT(quit()));
                GameOverLayout->addWidget(game_over);
                GameOverLayout->addWidget(reason);
                GameOverLayout->addWidget(restart);
                QWidget* GameOver = new QWidget;
                GameOver->setLayout(GameOverLayout);
                GameOver->show();
            }
         }
    }
}
/**
 * @brief GameBoard::update_monsters
 * @details updates the monsters' positions by using random number generators
 */
void GameBoard::update_monsters() {

    std::uniform_real_distribution<double> p(0,1);
    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);

    for(size_t i=0;i<number_monster;i++) {

        int px = monster_positions[i].rx();
        int py = monster_positions[i].ry();
        //std::cout<<px<<py<<std::endl;

        if(px>=0 && py >= 0 && px<board_size && py<board_size && (!(labels[py*board_size+px]->is_wall)) && (!(labels[py*board_size+px]->is_coin))
            && (!(labels[py*board_size+px]->is_cherry)) && (!(labels[py*board_size+px]->is_diamond))) {
            labels[py*board_size+px]->clear();
            labels[py*board_size+px]->setStyleSheet("QLabel { background-color : black; color: black;}");
        }

        int nx = px;
        int ny = py;

        // If it is time to move this monster, then
        if(p(generator) < 0.1) {

            if(x_or_y(generator) == 0) {
                nx = px + 2*increment(generator)-1;
                if(nx > -1 && nx < (int)board_size){
                    monster_positions[i].setX(nx);
                }
            }
            else {
                ny = py + 2*increment(generator)-1;
                if(ny > -1 && ny < (int)board_size){
                    monster_positions[i].setY(ny);
                }
            }
        }
    }

    repaint();
    QCoreApplication::processEvents();
}

/**
 * @brief GameBoard::create_wall
 * @details creates the walls in the board by selecting particular labels and reconstructing them with new GameBoardPieces which set the bool propert is_wall to be true
 */
void GameBoard::create_wall(){
    for(size_t i=0;i<4;i++){
        size_t wall_pos = 6*board_size + i;
        delete labels[wall_pos];
        labels[wall_pos] = new GameBoardPieces(Board, wall_pos, 1, 0, 0, 0);
        SquareGrid -> addWidget(labels[wall_pos] ,6,i);
    }
    for(size_t i=board_size/2;i<board_size;i++){
        size_t wall_pos = i*board_size + 5;
        delete labels[wall_pos];
        labels[wall_pos] = new GameBoardPieces(Board, wall_pos, 1, 0, 0, 0);
        SquareGrid -> addWidget(labels[wall_pos] ,i,5);
    }
    for(size_t i=8;i<board_size;i++){
        size_t wall_pos = 8*board_size + i;
        delete labels[wall_pos];
        labels[wall_pos] = new GameBoardPieces(Board, wall_pos, 1, 0, 0, 0);
        SquareGrid -> addWidget(labels[wall_pos] ,8,i);
    }
    for(size_t i=0;i<4;i++){
        size_t wall_pos = i*board_size + 7;
        delete labels[wall_pos];
        labels[wall_pos] = new GameBoardPieces(Board, wall_pos, 1, 0, 0, 0);
        SquareGrid -> addWidget(labels[wall_pos] ,i,7);
    }
    for(size_t i=5;i<8;i++){
        size_t wall_pos = 3*board_size + i;
        delete labels[wall_pos];
        labels[wall_pos] = new GameBoardPieces(Board, wall_pos, 1, 0, 0, 0);
        SquareGrid -> addWidget(labels[wall_pos] ,3,i);
    }
}
/**
 * @brief GameBoard::create_coin
 * @details generate random locations on the board for the coins, and construct the GameBoardPieces which set the bool property is_coin to be true
 */
void GameBoard::create_coin(){
    //use a set as container first to ensure unique locations for each coin
    std::set<int> coin_pos_set;
    while(coin_pos_set.size()<num_coins){
        int pos = rand()%(board_size*board_size);
        //put coin there only if the position is not occupied
        if((!(labels[pos]->is_wall)) &&  (!(labels[pos]->is_diamond)) && (!(labels[pos]->is_cherry)) && (pos!=0)){
            coin_pos_set.insert(pos);
        }
    }
    //convert the container of the coin positions from set to vector
    std::vector<int> coin_pos(coin_pos_set.begin(), coin_pos_set.end());

    for(size_t i=0; i<coin_pos.size(); i++){
        delete labels[coin_pos[i]];

        labels[coin_pos[i]] = new GameBoardPieces(Board, coin_pos[i], 0, 1, 0, 0);
        //calculate out the accordingly row number and column number the coin position is at
        int row = (coin_pos[i]+1)/board_size;
        int column = ((coin_pos[i]+1)%board_size)-1;
        if((coin_pos[i]+1)%board_size == 0){
            row = ((coin_pos[i]+1)/board_size) -1;
            column = board_size -1;
        }
        SquareGrid -> addWidget(labels[coin_pos[i]] ,row, column);
    }
}
/**
 * @brief GameBoard::create_diamond
 * @details generate random locations on the board for the diamond, and construct the GameBoardPieces which set the bool property is_diamond to be true
 */
void GameBoard::create_diamond(){
    //use a set as container first to ensure unique locations for each diamond
    std::set<int> diamond_pos_set;
    while(diamond_pos_set.size()<num_diamonds){
        int pos = rand()%(board_size*board_size);
        //put diamond there only if the position is not occupied
        if((!(labels[pos]->is_wall)) &&  (!(labels[pos]->is_coin)) && (!(labels[pos]->is_cherry)))
            diamond_pos_set.insert(pos);
    }
    std::vector<int> diamond_pos(diamond_pos_set.begin(), diamond_pos_set.end());
    for(int i=0; i<diamond_pos.size(); i++){
        delete labels[diamond_pos[i]];
        //construct GameBoardPices with is_diamond set to be true
        labels[diamond_pos[i]] = new GameBoardPieces(Board, diamond_pos[i], 0, 0, 1, 0);
        //calculate out the accordingly row number and column number the diamond position is at
        int row = (diamond_pos[i]+1)/board_size;
        int column = ((diamond_pos[i]+1)%board_size)-1;
        if((diamond_pos[i]+1)%board_size == 0){
            row = ((diamond_pos[i]+1)/board_size) -1;
            column = board_size -1;
        }
        SquareGrid -> addWidget(labels[diamond_pos[i]] ,row, column);
    }
}
/**
 * @brief GameBoard::create_cherry
 * @details generate random locations on the board for the cherries, and construct the GameBoardPieces which set the bool property is_cherry to be true
 */
void GameBoard::create_cherry(){

    std::set<int> cherry_pos_set;
    while(cherry_pos_set.size()<num_cherries){
        int pos = rand()%(board_size*board_size);
        //put cherry there only if the position is not occupied
        if((!(labels[pos]->is_wall)) &&  (!(labels[pos]->is_diamond)) && (!(labels[pos]->is_coin)))
            cherry_pos_set.insert(pos);
    }
    std::vector<int> cherry_pos(cherry_pos_set.begin(), cherry_pos_set.end());
    for(int i=0; i<cherry_pos.size(); i++){
        delete labels[cherry_pos[i]];
        labels[cherry_pos[i]] = new GameBoardPieces(Board, cherry_pos[i], 0, 0, 0, 1);
        int row = (cherry_pos[i]+1)/board_size;
        int column = ((cherry_pos[i]+1)%board_size)-1;
        if((cherry_pos[i]+1)%board_size == 0){
            row = ((cherry_pos[i]+1)/board_size) -1;
            column = board_size -1;
        }
        SquareGrid -> addWidget(labels[cherry_pos[i]] ,row, column);
    }
}
