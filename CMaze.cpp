/*
	File Id : CMaze.cpp
	Author : Riley Fewell
	Description : This implements everything that will take place in 
	the window. It will generate the maze, include the characters,
	display what will be in the instructions and cangratulations
	windows, and allow the user to move.
*/


#include <afxwin.h>
#include <afx.h>
#include "CMaze.h"
#include <time.h>
#include <algorithm>
#include <cstring>


using namespace std;



CMaze::CMazeSquare::CMazeSquare ()
{
// This function will diclare what a wall is in the maze:

	what = BRICK_WALL;
}

void CMaze::CMazeSquare::Display (CDC * deviceContextP)
{
// This function will display what is the characters and walls
	// will look like as they are moved and when the user reaches
	// then end of the maze:

	// Include your choice of colors and images here
		int red, green, blue;
		switch (what)
		{
			case WALL:// default wall; Color is black
				red = green = blue = 0;
				ImageName = "BRICKWALL_BMP";
				break;
			case EMPTY://Maze pathway
				red = green = blue = 255;
				ImageName = "SIDE_WALK_BMP";
				break;
			case ME:// default blue square
				ImageName = "";
				red = green = 0;
				blue = 255;
				break;
			case GOAL:// default red square
				ImageName = "";
				green = blue = 0;
				red = 255;
				break;
			case CRUMB:// pathway taken; default grey
				red = green = blue = 200;
				ImageName = "";
				break;
			case BRICK_WALL:// walls for the maze
				red = green = blue = 255;
				ImageName = "BRICK_WALL_BMP";
				break;
			case ROBBER:// The users character
				red = green = blue = 255;
				ImageName = "ROBBER_BMP";
				break;
			case MONEY_TRAIL:// the crumb
				red = green = blue = 200;
				ImageName = "MONEY_TRAIL_BMP";
				break;
			case GET_AWAY_CAR:// the goal
				red = green = blue = 255;
				ImageName = "GET_AWAY_CAR_BMP";
				break;
					
		}
		CBrush paintBrush;
		paintBrush.CreateSolidBrush (RGB(red, green, blue));
		CBrush * pBrushSv = deviceContextP->SelectObject (&paintBrush);
		deviceContextP->Rectangle (where);
		deviceContextP->SelectObject (pBrushSv);
		
	// Allows for a bitmap to be inserted into the program
	if (BRICK_WALL || ROBBER || MONEY_TRAIL || GET_AWAY_CAR)
	{
		CBitmap Image;
		int res = Image.LoadBitmapA(ImageName.c_str());
		CDC memDC;
		memDC.CreateCompatibleDC(deviceContextP);
		memDC.SelectObject(&Image);
		deviceContextP->TransparentBlt (where.left+1, where.top+1, where.Width()-2,
			where.Height()-2, &memDC, 0, 0, 80, 80, SRCCOPY);
	}
}

CMaze::CMaze ()
{
// This function is a default constructor and will declare all of the variables
	// as they change:

        numRows = 1;
        numCols = 1;
        grid = new CMazeSquare * [1];
        grid[0] = new CMazeSquare [1];
		finished = false;
        currentRow = 0;
        currentCol = 0;
        endRow = 0;
        endCol = 0;
}

CMaze::~CMaze ()
{
// This function will delete the maze when called upon.

	for (int r = 0; r < numRows; r++)
		delete [] grid[r];
	delete [] grid;
}

void CMaze::Init (int R, int C)
{
// This function will: declare everything inside the maze It will make new squares
	// fill in the boarder of the maze, and then call the generate function
	// to draw the maze, which will be ramdom each time

        numRows = R;
        numCols = C;
		// Declares grid to contain the number of rows and columns
        grid = new CMazeSquare * [numRows];
        for (int r = 0; r < numRows; r++)
            grid[r] = new CMazeSquare [numCols];
		// draws the boarder of the maze.
		for (int i = 1; i < numRows - 1; ++i)
		for (int j = 1; j < numCols - 1; ++j)
			grid[i][j].what = EMPTY;

		
		// function call for generating the maze 
		Generate (0, 0, numCols - 1, numRows - 1);
		finished = false;
        currentRow = 1;
        currentCol = 1;
		// places the user in the top left corner
        grid [currentRow][currentCol].what = ROBBER;
        endRow = numRows-2;
        endCol = numCols-2;
		//places the Goal in the bottom right corner
        grid [endRow][endCol].what = GET_AWAY_CAR;
}

void CMaze::Instructions (CFrameWnd * windowP)
{
// This function will: display the instructions of the game

	windowP->MessageBox ("How to play!\nUse the arrow keys (up, down, left, right) to move the robber(you)\ntowards the get away car (goal).\n Good luck!",
		"Instructions");
}

void CMaze::Display (CFrameWnd * windowP)
{
// This function will: display the size of the squares that will be used
	// onto the designated window

		CPaintDC dc (windowP);
		CRect rect;
		windowP->GetClientRect (&rect);
		dc.SetBkMode(TRANSPARENT);
		int sqWidth = rect.Width() / numCols;
		int sqHeight = rect.Height() / numRows;
		int ulx, uly = (rect.Height() - numRows * sqHeight) / 2;
		int r, c;
		for (r = 0; r < numRows; r++)
		{
			ulx = (rect.Width() - numCols * sqWidth) / 2;
			for (c = 0; c < numCols; c++)
			{
				grid [r][c].where = CRect (ulx, uly, ulx + sqWidth, uly + sqHeight);
				ulx += sqWidth;
			}
			uly += sqHeight;
		}
		for (r = 0; r < numRows; r++)
			for (c = 0; c < numCols; c++)
				grid [r][c].Display (&dc);
}

CRect CMaze::Move (dType direction)
{
// This function will: Declare what the arrow keys will do when pressed.
	// It allows the user to move left when they push the left arrow key,
	// move right when the user pushes the right arrow key and so on.

		CRect from, to;
		from = grid[currentRow][currentCol].where;// the current position of the user
		to = grid[currentRow][currentCol].where;// the square the user wants to be switched to.
        switch (direction)
        {
			case LEFT:
				if (direction == LEFT && grid[currentRow][currentCol - 1].what != BRICK_WALL)
				{
					swap( grid[currentRow][currentCol - 1].what = MONEY_TRAIL, grid[currentRow][currentCol].what);
					to = grid[currentRow][currentCol - 1].where;
					--currentCol;
				}
				break;
			case UP:
				if (direction == UP && grid[currentRow - 1][currentCol].what != BRICK_WALL)
				{
					swap(grid[currentRow - 1][currentCol].what = MONEY_TRAIL, grid[currentRow][currentCol].what);
					to = grid[currentRow - 1][currentCol].where;
					--currentRow;
				}
				break;
			case RIGHT: 
				if (direction == RIGHT && grid[currentRow][currentCol + 1].what != BRICK_WALL)
				{
					swap(grid[currentRow][currentCol + 1].what = MONEY_TRAIL, grid[currentRow][currentCol].what);
					to = grid[currentRow][currentCol + 1].where;
					++currentCol;
				}
				break;
			case DOWN:
				if (direction == DOWN && grid[currentRow + 1][currentCol].what != BRICK_WALL)
				{
					swap( grid[currentRow + 1][currentCol].what = MONEY_TRAIL, grid[currentRow][currentCol].what);
					to = grid[currentRow + 1][currentCol].where;
					++currentRow;
				}
				break;
        }
        
		// Lets the game know that the user has reached the goal
		// meaning the user has won.
		if (currentRow == endRow && currentCol == endCol)
		{
			finished = true;
		
			if (grid[currentRow - 1][currentCol].what == GET_AWAY_CAR)
				grid[currentRow - 1][currentCol].what = MONEY_TRAIL;
			if (grid[currentRow][currentCol - 1].what == GET_AWAY_CAR)
				grid[currentRow][currentCol - 1].what = MONEY_TRAIL;
		}

		return from | to;
		

}

void CMaze::Message (CFrameWnd * windowP)
{
// This function will: display the window that congratulates the user for completing the maze.

	windowP->MessageBox ("Congratulations!!!\nYou have gotten away with all the money!!\n",
		"Winner");
}


void CMaze::Generate (int left, int top, int right, int bottom)
{
// This function generates a random maze by using a method called recurrsize division.
	// It draws two lines and creates 3 doors until there is no more space to divide.
	srand (time(NULL));
	
	// allows the recurrsion to stop when there is no more space to divide
	if (right - left <= 3 || bottom - top <= 3)
		return;

	int vert = 0;
	int horiz = 0;

	// algorithm for finding where the walls should be drawn
	vert = 2 * (rand () % ((right - left - 2) / 2)) + 2 + left;
	horiz = 2 * (rand () % (( bottom - top - 2) / 2)) + 2 + top;

	// Draws the walls
	for (int i = top; i < bottom; ++i)
		grid[i][vert].what = BRICK_WALL;
	for (int i = left; i < right; ++i)
		grid[horiz][i].what = BRICK_WALL;

	//algorithm for drawing the doors
	int door1 = 2 * (rand () % ((horiz - top) / 2)) + 1 + top;
	int door2 = 2 * (rand () % ((vert - left) / 2)) + 1 + left;
	int door3 = 2 * (rand () % ((bottom - horiz) / 2)) + 1 + horiz;
	int door4 = 2 * (rand () % ((right - vert) / 2)) + 1 + vert;

	int skip = rand () % 4;

	// draws the doors
	if (skip != 0)
		grid[door1][vert].what = EMPTY;
	if (skip != 1)
		grid[horiz][door2].what = EMPTY;
	if (skip != 2)
		grid[door3][vert].what = EMPTY;
	if (skip != 3)
		grid[horiz][door4].what = EMPTY;
		
	// where recurrsive step continues
	Generate (left, top, vert, horiz);
	Generate (vert, top, right, horiz);
	Generate (left, horiz, vert, bottom);
	Generate (vert, horiz, right, bottom);
}



