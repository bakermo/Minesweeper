#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Tile.h"
#include "Random.h"
#include "MenuButton.h"

using namespace std;

struct BoardConfig
{
	int columns;
	int rows;
	int mines;
};

class Board
{
	int columns;
	int rows;
	int mines;
	bool gameOver;
	const int tileSize = 32;
	const int menuDepth = 88;
	int menuYPosition;

	MenuButton gameBtn;
	MenuButton debugMineBtn;
	MenuButton debugButtons[3];
	map<pair<int, int>, Tile> tiles;

	pair<int, int> GetTileKey(int xPos, int yPos);
	BoardConfig LoadFromConfig();
	void LoadTestScenario(string board);
	void CreateMenu();
	void NewGame();
	void InitializeMines();
	void Win();
	void Lose();
public:
	Board();
	//todo: review which of these need to actually be public or not
	int GetColumns();
	int GetRows();
	int GetMineCount();
	int GetHeight();
	int GetWidth();
	void OnClick(sf::Event::MouseButtonEvent mouseButtonEvent);
	void Render(sf::RenderWindow& window);
	void ToggleCheat();
};

