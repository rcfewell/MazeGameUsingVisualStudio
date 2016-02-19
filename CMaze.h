/*
	File ID : CMaze.h
	Author : Riley Fewell
*/

#define _AFXDLL

#ifndef CMAZE_H
#define CMAZE_H

#define _AFXDLL

#include <afxwin.h>
#include <string>
using namespace std;

enum sType {WALL, EMPTY, ME, GOAL, CRUMB, BRICK_WALL, ROBBER, MONEY_TRAIL, GET_AWAY_CAR};
enum dType {UP, DOWN, LEFT, RIGHT};

class CMaze
{
	public:
        CMaze ();
        CMaze (const CMaze & other);
        ~CMaze ();
        CMaze & operator = (const CMaze & other);
		void Init (int R, int C);
        void Instructions (CFrameWnd * windowP);
        void Display (CFrameWnd * windowP);
        CRect Move (dType direction);
        void Message (CFrameWnd * windowP);
		bool finished;
    private:
		struct CMazeSquare
		{
		    CMazeSquare ();
			void Display (CDC * deviceContextP);
			CRect where;
			sType what;
			string ImageName;
		};
        void Generate (int left, int top, int right, int bottom);
        CMazeSquare ** grid;
        int numRows, numCols;
        int currentRow, currentCol;
        int endRow, endCol;
};

#endif