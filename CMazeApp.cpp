/*
	File Id : CMazeApp.cpp
	Author : Riley Fewell
	Description : This implements how the window will update 
	everytime something happens.
*/


#include <afxwin.h>
#include <afx.h>
#include "CMazeApp.h"
using namespace std;


BOOL CMazeApp::InitInstance ()
{
// This function will display everything that will display the 
	// widnow and it will update the widow everytime something happens:

		m_pMainWnd = new CMazeWin();
		m_pMainWnd->ShowWindow (m_nCmdShow);
		m_pMainWnd->UpdateWindow ();

		return TRUE;
}

CMazeApp MazeApp;