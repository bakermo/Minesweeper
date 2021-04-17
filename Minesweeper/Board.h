#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.h"
using namespace std;


class Board
{
	int columns;
	int rows;
	int mines;
	const int tileSize = 32;
	const int menuDepth = 88;
	int menuYPosition;
	map<pair<int, int>, Tile> tiles;
	pair<int, int> GetTileKey(int xPos, int yPos);

public:
	Board(int columns, int rows, int mines);

	//todo: review which of these need to actually be public or not
	int GetColumns();
	int GetRows();
	int GetMineCount();
	int GetHeight();
	int GetWidth();
	void OnClick(sf::Event::MouseButtonEvent mouseButtonEvent);
	void Render(sf::RenderWindow& window);
};

