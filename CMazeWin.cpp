/*
	File Id : CMazeWin.cpp
	Author : Riley Fewell
	Description : This implements the window. How big it will
	be, what will happen when a key is pushed and how the window
	will be painted.
*/

#include <afxwin.h>
#include "CMazeWin.h"
using namespace std;

CMazeWin::CMazeWin ()
{
// This function will create the window for the maze that will display
	// the maze and will also include a title at the top of the screen:
		Height = 11;
		Width = 11;
		Create (NULL, "Maze");
		myMaze.Init(Height, Width);
		firstTime = true;
}

afx_msg void CMazeWin::OnPaint ()
{
// This function will make a display window that will
	// display the instructions for the maze.:

		myMaze.Display (this);
		if (firstTime)
		{
			myMaze.Instructions(this);
			firstTime = false;
		}
}

afx_msg void CMazeWin::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
// This function will allow the character that the user controls to be 
	// to move up, down, right and left inorder to let the user reach the
	// end:

	CRect change;
	switch (nChar)
	{
	
	case 37: // Left arrow key
			change = myMaze.Move(LEFT);
			InvalidateRect (change, TRUE);
			break;
	case 38: // Up arrow key
			change = myMaze.Move(UP);
			InvalidateRect (change, TRUE);
			break;
	case 39: // Right arrow key
			change = myMaze.Move(RIGHT);
			InvalidateRect (change, TRUE);
			break;
	case 40: // Down arrow key
			change = myMaze.Move(DOWN);
			InvalidateRect (change, TRUE);
			break;
	default:
		MessageBox ("Key not recognized");
	}
	if (myMaze.finished)
	{
		Height += 2;
		Width += 2;
		myMaze.Message (this);
		myMaze.Init(Height, Width);
		Invalidate (TRUE);
	}
}

BEGIN_MESSAGE_MAP (CMazeWin, CFrameWnd)
	ON_WM_PAINT ()
	ON_WM_KEYDOWN( )
END_MESSAGE_MAP ()
