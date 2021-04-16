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
	vector<Tile> tiles;

public:
	Board(int columns, int rows, int mines);

	//todo: review which of these need to actually be public or not
	int GetColumns();
	int GetRows();
	int GetMineCount();
	int GetHeight();
	int GetWidth();
	void Render(sf::RenderWindow& window);
};

