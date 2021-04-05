#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#include "olcConsoleGameEngine.h"

class FroggerConsole : public olcConsoleGameEngine
{
public:
	FroggerConsole()
	{
		m_sAppName = L"Frogger";
	}
private:
	vector<pair<float, wstring>> vecLanes =
	{
		//Velocity >0 means -> , Velocity <0 means <- else if Velocity = 0 , nothing needs to move
		//String L" " needed to know if it's safe or not safe to step up (./x)
		{0.0f,  L"pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"}, // 64 elements per lane
		{-3.0f, L",,,jllk,,jllllk,,,,,,,jllk,,,,,jk,,,jlllk,,,,jllllk,,,,jlllk,,,,"},
		{3.0f,  L",,,,jllk,,,,,jllk,,,,jllk,,,,,,,,,jllk,,,,,jk,,,,,,jllllk,,,,,,,"},
		{2.0f,  L",,jlk,,,,,jlk,,,,,jk,,,,,jlk,,,jlk,,,,jk,,,,jllk,,,,jk,,,,,,jk,,"},
		{0.0f,  L"pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"},
		{-3.0f, L"....asdf.......asdf....asdf..........asdf........asdf....asdf..."},
		{ 3.0f, L".....ty..ty....ty....ty.....ty........ty..ty.ty......ty.......ty"},
		{-4.0f, L"..zx.....zx.........zx..zx........zx...zx...zx....zx...zx...zx.."},
		{2.0f,  L"..ty.....ty.......ty.....ty......ty..ty.ty.......ty....ty......."},
		{0.0f,  L"pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"}

	};

	float fTimeSinceStart = 0.0f;
	int nCellSize = 8;

	olcSprite* spriteBus = nullptr;
	olcSprite* spriteLog = nullptr;
	olcSprite* spriteWater = nullptr;
	olcSprite* spriteFrog = nullptr;
	olcSprite* spritePavement = nullptr;
	olcSprite* spriteCar1 = nullptr;
	olcSprite* spriteCar2 = nullptr;
	olcSprite* spriteWall = nullptr;
	olcSprite* spriteHome = nullptr;

protected:
	virtual bool OnUserCreate()
	{
		spriteBus = new olcSprite(L"../Sprites/bus.spr");
		spriteLog = new olcSprite(L"../Sprites/log.spr");
		spriteWater = new olcSprite(L"../Sprites/water.spr");
		spriteFrog = new olcSprite(L"../Sprites/frog.spr");
		spritePavement = new olcSprite(L"../Sprites/pavement.spr");
		spriteCar1 = new olcSprite(L"../Sprites/car1.spr");
		spriteCar2 = new olcSprite(L"../Sprites/car2.spr");
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		std::ofstream Log;
		Log.open("Logfile.txt", std::ios_base::app);

		fTimeSinceStart += fElapsedTime;
		int x = -1, y = 0;

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
		//Draw Lanes
		for (auto lane : vecLanes)
		{
			//Find offset
			int nStartPos = (int)(fTimeSinceStart * lane.first) % 64; // X(t) = V.t ;
			if (nStartPos < 0)	nStartPos = 64 - (abs(nStartPos) % 64);
			//Log << " Velocity = " << lane.first << " Start position = " << nStartPos << "\n";

			int nCellOffset = (int)(8.0f * fTimeSinceStart * lane.first) % nCellSize;
		     
			//Draw 
			for (int i = 0; i < (ScreenWidth() / nCellSize)+2; i++)
			{
				wchar_t graphic = lane.second[(nStartPos + i) % 64];
			
				switch (graphic)
				{
				case L'a':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteBus, 0, 0, 8, 8);		break; // Bus 1
				case L's':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteBus, 8, 0, 8, 8);		break; // Bus 2
				case L'd':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteBus, 16, 0, 8, 8);	break; // Bus 3
				case L'f':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteBus, 24, 0, 8, 8);	break; // Bus 4

				case L'j':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteLog, 0, 0, 8, 8);		break; // Log Start
				case L'l':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteLog, 8, 0, 8, 8);		break; // Log Middle
				case L'k':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteLog, 16, 0, 8, 8);	break; // Log End

				case L'z': 	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteCar1, 0, 0, 8, 8);	break; // Car1 Back
				case L'x':	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteCar1, 8, 0, 8, 8);	break; // Car1 Front

				case L't': 	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteCar2, 0, 0, 8, 8);	break; // Car2 Back
				case L'y': 	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteCar2, 8, 0, 8, 8);	break; // Car2 Front

				case L',': 	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spriteWater, 0, 0, 8, 8);	break; // Water
				case L'p': 	DrawPartialSprite((x + i) * nCellSize - nCellOffset, y * nCellSize, spritePavement, 0, 0, 8, 8); break; // Pavement
				case L'.': 	Fill((x + i) * nCellSize - nCellOffset, y * nCellSize, (x + i + 1) * nCellSize - nCellOffset, (y + 1) * nCellSize, L' ');

			    }
			
			}

			y++;
		}

		return true;
	}
};

int main()
{
	FroggerConsole game;
	game.ConstructConsole(128, 80, 8, 8); //80 Width , 100 Height , 8x8 pixels
	game.Start();


	return 0;
}