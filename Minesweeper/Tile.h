#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Tile
{
	int column;
	int row;
	bool hasMine;
	bool isRevealed;
	bool isFlagged;
	void DrawLayer(sf::RenderWindow& window, string texture);
public:
	Tile(int column, int row, bool hasMine);
	int XPos();
	int YPos();

	bool IsRevealed();
	bool IsFlagged();
	bool HasMine();

	void Reveal();
	void ToggleFlag();
	void Render(sf::RenderWindow& window);
};

