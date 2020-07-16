#ifndef GAMECANVAS_H
#define GAMECANVAS_H
#include <opencv2/opencv.hpp>

enum class Piece { human, bot, none};
enum class Winner { human, bot, draw, uncertain}; //uncertain = game not end yet

// move of the piece, for exsample:
// Move(0,0)    Move(1,0)    Move(2,1)
// O | - | -    - | O | -    - | - | -
// - | - | -    - | - | -    - | - | O
// - | - | -    - | - | -    - | - | -
struct Move
{
    int x;
    int y;
public:
    Move();
    Move(int a,int b);
};

// A TieTacToe game that let user play against computer, display game with opencv
class GameCanvas
{
private:
    const int def_width = 600; // The default resolution of the game-design
    const int def_high = 760; // The default resolution of the game-design
    int width = 600; // The actual display width of the game (change base on user's screen width)
    std::string windowName = "Tie Tac Toe"; // The name of the game window
    Piece board[3][3]; // Record what pieces are on the board
    cv::Mat canvas; // The canvas where we draw all the text, game board, piece
    Winner winner; // Record the winner of current game (use to determine whether the game is still progress or already end)
    bool nextMoveBOT; // If human moved first and draw in the last game,bot move first in the next game

public:
    GameCanvas(std::string name); //input : what name you want to use as the window name. (default:Tie Tac Toe)
    GameCanvas();

    // Main game funcion, get and check player's input then progress one round then wait for next input
    // Input: coordinate where player click on the game display window
    void playerInput(int,int);

    //get human/bot's move, place the piece on the board and draw it on the canvas
    //check whether there is a winner, if so then end the game
    void humanMove(Move);
    void botMove();

    // Check and return the winner of current board, if no winner yet, return "Winner::uncertain"
    Winner checkWinner();

    // Check and return whether input move is a legit move
    bool checkMoveLegit(Move);

    // Start a newgame, clear the canvas, the board then draw a new board and display it
    void newGame();

    // Game end, draw outcome of the game and wait user to start a newgame
    void gameEnd();

    // Returns bot's best move via the minimax algorithm. If there are multiple best moves, randomly return one
    Move miniMax();

    // miniMax algorithm's sub function, return the score of current choice move
    int minSearch();
    int maxSearch();

    // Draw gameboard on canvas
    void drawBoard();

    // Draw O,X on canvas
    // Input: the position of O,X we need to draw on
    void drawO(Move);
    void drawX(Move);

    // Resize the game-canvas to fit user's screen and then display it
    void displayCanvas();

    // get the resolution of user's screen and set the display width base on it;
    void getDisplayWidth();
};

#endif // GAMECANVAS_H
