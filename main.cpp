#include <iostream>
#include "GameCanvas.h"

void onMouse(int event, int x, int y, int flags, void* param );// opencv mousehandler
GameCanvas TieTacToe;// create the "Tie Tac Toe game" object

int main()
{
    cv::setMouseCallback("Tie Tac Toe", onMouse, NULL);// get user mouse input
    waitKey(0);
    return 0;
}

void onMouse(int event, int x, int y, int flags, void* param )
{
    if (event == EVENT_LBUTTONDOWN) //player right click => input human's next move
    {
        TieTacToe.playerInput(x,y);
    }
}
