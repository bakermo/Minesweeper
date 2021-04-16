#include "Board.h"
Board::Board(int columns, int rows, int mines)
{
	this->columns = columns;
	this->rows = rows;
	this->mines = mines;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			Tile tile(j, i, false);
			tiles.push_back(tile);
		}
	}
}

int Board::GetColumns()
{
	return columns;
}

int Board::GetRows()
{
	return rows;
}

int Board::GetMineCount()
{
	return mines;
}

//todo: make private?
int Board::GetHeight()
{
	//todo: consider if there should be a height just for tiles and a height just for window
	return (rows * 32) + 88;
}

//todo: make private?
int Board::GetWidth()
{
	return columns * 32;
}

void Board::Render(sf::RenderWindow& window)
{
	for (Tile t : tiles)
	{
		t.Render(window);
	}

	int menuYPos = GetHeight() - 88;

	sf::Sprite counterSprite(TextureManager::GetTexture("digits"));
	counterSprite.setPosition(0, menuYPos);
	window.draw(counterSprite);

	int menuXPos = (GetWidth() / 2) - 32;
	sf::Sprite faceTileSprite(TextureManager::GetTexture("face_happy"));
	faceTileSprite.setPosition(menuXPos, menuYPos);
	window.draw(faceTileSprite);

	menuXPos += 96; // 32 pixels for rest of face button, plus 64 pixels of space
	sf::Sprite debugSprite(TextureManager::GetTexture("debug"));
	debugSprite.setPosition(menuXPos, menuYPos);
	window.draw(debugSprite);

	for (unsigned int i = 1; i <= 3; i++)
	{
		menuXPos += 64; // width of each button
		sf::Sprite testSprite(TextureManager::GetTexture("test_" + std::to_string(i)));
		testSprite.setPosition(menuXPos, menuYPos);
		window.draw(testSprite);
	}
}




