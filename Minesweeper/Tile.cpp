#include "Tile.h"
Tile::Tile(int column, int row, bool mine)
{
	this->column = column;
	this->row = row;
	hasMine = mine;
	isFlagged = false;
	isRevealed = false;
}

int Tile::XPos()
{
	return column * 32;
}

int Tile::YPos()
{
	return row * 32;
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

	// then take some action
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
			DrawLayer(window, "flagged");
		}
	}
}

void Tile::DrawLayer(sf::RenderWindow& window, string texture)
{
	sf::Sprite sprite(TextureManager::GetTexture(texture));
	sprite.setPosition(XPos(), YPos());
	window.draw(sprite);
}