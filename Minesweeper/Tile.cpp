#include "Tile.h"
Tile::Tile(int column, int row)
{
	this->column = column;
	this->row = row;
	adjacentMines = 0;
	hasMine = false;
	isFlagged = false;
	isRevealed = false;
	isLosingTile = false; // flags this tile as the losing tile when the game ends
	showMine = false;
}

void Tile::DrawLayer(sf::RenderWindow& window, string texture, bool losingTile)
{
	sf::Sprite sprite(TextureManager::GetTexture(texture));
	if (losingTile)
	{
		sprite.setColor(sf::Color::Red);
	}
	sprite.setPosition(XLeftBound(), YUpperBound());
	window.draw(sprite);
}

int Tile::XLeftBound()
{
	return column * tileWidth;
}

int Tile::XRightBound()
{
	return XLeftBound() + tileWidth;
}

int Tile::YUpperBound()
{
	return row * tileWidth;
}

int Tile::YLowerBound()
{
	return YUpperBound() + tileWidth;
}

bool Tile::HasMine()
{
	return hasMine;
}

bool Tile::IsRevealed()
{
	return isRevealed;
}

bool Tile::IsFlagged()
{
	return isFlagged;
}

void Tile::SetAdjacentTiles(std::vector<Tile*> adjacentTiles) 
{
	//ok, theres probably a better way to do this
	// but its an order of ops problem
	// you need the tiles to exist before you know what is adjacent to them
	this->adjacentMines = 0;
	for (Tile* t : adjacentTiles)
	{
		if (t->HasMine())
		{
			this->adjacentMines++;
		}
	}
	this->adjacentTiles = adjacentTiles;
}

void Tile::PlaceMine()
{
	hasMine = true;
}


void Tile::Reveal()
{
	if (!isFlagged && !isRevealed)
	{
		isRevealed = true;
		if (hasMine)
		{
			isLosingTile = true;
		}
	}
}

void Tile::ToggleFlag()
{
	isFlagged = !isFlagged;
}

void Tile::ShowMine()
{
	if (hasMine)
	{
		isRevealed = true;
		showMine = true;
	}
}

void Tile::ToggleCheat()
{
	showMine = !showMine;
}

void Tile::Render(sf::RenderWindow& window)
{
	if (isRevealed)
	{
		DrawLayer(window, "tile_revealed", isLosingTile);
		if (hasMine)
		{
			DrawLayer(window, "mine");
		} 
		else if (adjacentMines > 0 && adjacentMines <= 8)
		{
			DrawLayer(window, "number_" + std::to_string(adjacentMines));
		}
		//TODO: recursive reveal!
	}
	else
	{
		DrawLayer(window, "tile_hidden");
		if (isFlagged)
		{
			DrawLayer(window, "flag");
		}
	}

	if (showMine && hasMine)
	{
		DrawLayer(window, "mine");
	}
}
