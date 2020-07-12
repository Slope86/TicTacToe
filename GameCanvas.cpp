#include "GameCanvas.h"
#include <bits/stdc++.h>
#include <windows.h>

using namespace cv;

GameCanvas::GameCanvas()
{
    getDisplayWidth();
    newGame();
}

Move::Move()
{
    x=0;
    y=0;
}

Move::Move(int a,int b)
{
    x=a;
    y=b;
}

// What happen after got a user input:
// 0. check whether the game is end, if so start a new game
// 1. check whether the input is legit
// 2. place the piece on the board and draw it on the canvas
// 3. check whether there is a winner, if so then end the game
// 4. use miniMax algorithm to get computer's move
// 5. repeat step2. step3.
// 6. display user's and computer's move
void GameCanvas::playerInput(int x, int y)
{
    // if game ended then start a new game
    if(winner != Winner::uncertain)
    {
        newGame();
        if(nextMoveAI)// If human moved first and draw in the last game,computer move first in the next game
            aiMove();
        return;
    }

    // check input legit or not, if so then progress one turn
    Move playerMove(x*3/width,y*3/width); // pixal coordinate to board coordinate
    if(checkMoveLegit(playerMove))
    {
        humanMove(playerMove);
        if(winner == Winner::uncertain)// check game end or not after human's turn
            aiMove();
    }
}

void GameCanvas::humanMove(Move move)
{
    board[move.x][move.y]=Piece::human;
    drawO(move);
    winner = checkWinner();
    if(winner != Winner::uncertain)
    {
        nextMoveAI = true;
        gameEnd();
    }
    displayCanvas();
}

void GameCanvas::aiMove()
{
    Move move = miniMax();
    board[move.x][move.y]=Piece::ai;
    drawX(move);
    winner = checkWinner();
    if(winner != Winner::uncertain)
    {
        nextMoveAI = false;
        gameEnd();
    }
    displayCanvas();
}

void GameCanvas::newGame()
{
    winner = Winner::uncertain;
    canvas = Mat(def_width+160, def_width, CV_8UC3, Scalar(255,255,255));
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            board[i][j] = Piece::none;
        }
    }
    drawBoard();
    displayCanvas();
}

bool GameCanvas::checkMoveLegit(Move move)
{
    if(move.x>2 || move.x<0 || move.y >2 ||move.y<0)
        return false;
    return board[move.x][move.y]==Piece::none;
}

Winner GameCanvas::checkWinner()
{
    //Check whether there is three same pieces in a line, if so there is a winner
    for(int i=0; i<3; i++)
    {
        if(     board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][1] != Piece::none ||  // Check horizontals
                board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[1][i] != Piece::none )   // Check verticals
            return (board[i][i]==Piece::ai) ? (Winner::ai) : (Winner::human) ;
    }

    if(     board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != Piece::none || // Check diagonals
            board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != Piece::none )
        return (board[1][1]==Piece::ai) ? (Winner::ai) : (Winner::human) ;

    for(int i=0; i<3; i++) // Check board full or not -> "game draw" or "game not end yet"
    {
        if(board[i][0] == Piece::none || board[i][1]== Piece::none || board[i][2] == Piece::none)
            return Winner::uncertain;
    }
    return Winner::draw;
}

void GameCanvas::gameEnd()
{
    // draw "who win" on the canvas
    switch (winner)
    {
    case Winner::human :
        putText(canvas, std::string("human win !!!"), Point(def_width/10-15,def_width*7/6), 0, def_width/150, Scalar(0,255,0),3);
        break;
    case Winner::ai :
        putText(canvas, std::string("AI win !!!"), Point(def_width/10-def_width/24,def_width*7/6), 0, def_width/150, Scalar(0,0,255),3);
        break;
    case Winner::draw :
        putText(canvas, std::string("Draw"), Point(def_width/4-def_width/120,def_width*7/6), 0, def_width/150, Scalar(0,255,0),3);
        break;
    }
    // tell user press space to start new game and wait
    putText(canvas, std::string("right click to start a new game"), Point(def_width/10-def_width/60,def_width*5/4-10), 0, 1, Scalar(0,0,0),1);
    displayCanvas();
}

Move GameCanvas::miniMax()
{
    Move move[9]; // Record all the lowest-score moves
    int numMove = 0; // The amount of moves which share the same score
    int score = 2; // humanWin=1, aiWin=-1 , draw=0, max=2, min=-2

    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            if (board[i][j] == Piece::none)
            {
                board[i][j] = Piece::ai;
                int tmp = maxSearch();
                if (tmp < score)
                {
                    score = tmp;
                    numMove = 1;
                    move[numMove-1].x = i;
                    move[numMove-1].y = j;
                }
                else if(tmp == score)
                {
                    numMove++;
                    move[numMove-1].x = i;
                    move[numMove-1].y = j;
                }
                board[i][j] = Piece::none;
            }
        }
    }
    srand(time(NULL));
    int rand_i = rand() % numMove; //Randomly choose one of the lowest-score moves
    return move[rand_i];
}

int GameCanvas::maxSearch()
{
    switch (Winner winner = checkWinner())
    {
    case Winner::human :
        return 1;
    case Winner::ai :
        return -1;
    case Winner::draw :
        return 0;
    }

    int score = -2;
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            if (board[i][j] == Piece::none)
            {
                board[i][j] = Piece::human;
                score = std::max(score, minSearch());
                board[i][j] = Piece::none;
            }
        }
    }
    return score;
}

int GameCanvas::minSearch()
{
    switch (Winner winner = checkWinner())
    {
    case Winner::human :
        return 1;
    case Winner::ai :
        return -1;
    case Winner::draw :
        return 0;
    }

    int score = 2;
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            if (board[i][j] == Piece::none)
            {
                board[i][j] = Piece::ai;
                score = std::min(score, maxSearch());
                board[i][j] = Piece::none;
            }
        }
    }
    return score;
}

void GameCanvas::displayCanvas()
{
    cv::Mat resize_canvas;
    cv::resize(canvas, resize_canvas, cv::Size(), (float)width/def_width, (float)width/def_width);
    imshow("Tie Tac Toe", resize_canvas);
}

void GameCanvas::drawBoard()
{
    int thickness = def_width/100+1;
    int lineType = LINE_8;
    Scalar color = Scalar(0,0,0);
    line(canvas, Point(def_width/3,1+def_width/50), Point(def_width/3,def_width*49/50), color, thickness, lineType );
    line(canvas, Point(def_width*2/3,1+def_width/50), Point(def_width*2/3,def_width*49/50), color, thickness, lineType );
    line(canvas, Point(1+def_width/50,def_width/3), Point(def_width*49/50,def_width/3), color, thickness, lineType );
    line(canvas, Point(1+def_width/50,def_width*2/3), Point(def_width*49/50,def_width*2/3), color, thickness, lineType );
}

void GameCanvas::drawO(Move move)
{
    int thickness = def_width/100+1;
    int rad = def_width/10;
    Scalar color = Scalar(255,0,0);
    Point origin(1*def_width/6+move.x*def_width/3,1*def_width/6+move.y*def_width/3);
    circle(canvas, origin, rad, color, thickness);
}

void GameCanvas::drawX(Move move)
{
    int thickness = def_width/100+1;
    int lineType = LINE_8;
    Scalar color = Scalar(0,0,255);

    Point origin(def_width/6+move.x*def_width/3,def_width/6+move.y*def_width/3);
    line(canvas, Point(origin.x-def_width/10,origin.y-def_width/10),Point(origin.x+def_width/10,origin.y+def_width/10), color, thickness, lineType );
    line(canvas, Point(origin.x+def_width/10,origin.y-def_width/10),Point(origin.x-def_width/10,origin.y+def_width/10), color, thickness, lineType );
}

void GameCanvas::getDisplayWidth()
{
    int ScreenW  = (int) GetSystemMetrics(SM_CXSCREEN);
    int ScreenH = (int) GetSystemMetrics(SM_CYSCREEN);
    width = def_width*0.75*(float)std::min((float)ScreenW/def_width,(float)ScreenH/def_high) - 20;
}
