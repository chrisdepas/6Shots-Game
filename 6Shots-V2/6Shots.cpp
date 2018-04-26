// 6Shots.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CGame.h"
int _tmain(int argc, _TCHAR* argv[])
{
	CGame game;
	game.Initialise();

	while (!game.ShouldQuit()) {
		game.Step();
		game.Draw();
		game.HandleInput();
	}

	CDebugLogger::LogDebug("Quitting");
	return 0;
}

