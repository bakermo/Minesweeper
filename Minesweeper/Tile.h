#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureManager.h"

class Tile
{
	int column;
	int row;
	int adjacentMines;
	bool hasMine;
	bool isRevealed;
	bool isFlagged;
	bool isLosingTile;
	bool showMine;
	const int tileWidth = 32;
	std::vector<Tile*> adjacentTiles;
	void DrawLayer(sf::RenderWindow& window, string texture, bool losingTile = false);
public:
	Tile(int column, int row);
	int XLeftBound();
	int XRightBound();
	int YUpperBound();
	int YLowerBound();
	bool HasMine();
	bool IsRevealed();
	bool IsFlagged();

	void SetAdjacentTiles(std::vector<Tile*> adjacentTiles);
	void PlaceMine();
	int Reveal();
	void ToggleFlag();
	void ShowMine();
	void ToggleCheat();
	void Render(sf::RenderWindow& window);
};

