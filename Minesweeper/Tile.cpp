#include "Tile.h"
Tile::Tile(int column, int row, bool mine)
{
	this->column = column;
	this->row = row;
	hasMine = mine;
	isFlagged = false;
	isRevealed = false;
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

void Tile::Reveal()
{
	if (!isRevealed)
		isRevealed = true;
}

void Tile::ToggleFlag()
{
	isFlagged = !isFlagged;
}

void Tile::Render(sf::RenderWindow& window)
{
	if (isRevealed)
	{
		DrawLayer(window, "tile_revealed");
		if (hasMine)
		{
			DrawLayer(window, "mine");
		} 
		//TODO: handle numbers
	}
	else
	{
		DrawLayer(window, "tile_hidden");
		if (isFlagged)
		{
			DrawLayer(window, "flag");
		}
	}
}

void Tile::DrawLayer(sf::RenderWindow& window, string texture)
{
	sf::Sprite sprite(TextureManager::GetTexture(texture));
	sprite.setPosition(XLeftBound(), YUpperBound());
	window.draw(sprite);
}