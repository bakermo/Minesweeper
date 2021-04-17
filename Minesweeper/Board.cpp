#include "Board.h"
Board::Board(int columns, int rows, int mines)
{
	this->columns = columns;
	this->rows = rows;
	this->mines = mines;
	this->menuYPosition = rows * tileSize;

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			Tile tile(col, row, false);
			// key the map by column, row
			tiles.emplace(make_pair(col, row), tile);
		}
	}
}

/// <summary>
/// Returns the column/row of the tile
/// </summary>
/// <param name="xPos">x position of the column</param>
/// <param name="yPos">y position of the row</param>
/// <returns></returns>
pair<int, int> Board::GetTileKey(int xPos, int yPos)
{
	// we rely on truncation to get us the correct column/row
	// Ex: 63 / 32 = 1.9, .9 is truncated, column 1;
	return make_pair<int, int>(xPos / tileSize, yPos / tileSize);
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
	return (rows * 32) + menuDepth;
}

//todo: make private?
int Board::GetWidth()
{
	return columns * 32;
}

void Board::OnClick(sf::Event::MouseButtonEvent mouseButtonEvent)
{
	if (mouseButtonEvent.y < menuYPosition)
	{
		// mouse is within tile area
		// look up the tile by the column, row
		pair<int, int> tileKey = GetTileKey(mouseButtonEvent.x, mouseButtonEvent.y);
		if (mouseButtonEvent.button == sf::Mouse::Right)
		{
			tiles.at(tileKey).ToggleFlag();
		}
		else if (mouseButtonEvent.button == sf::Mouse::Left)
		{
			// we explicitly "else if" here so that 
			// other mouse buttons don't trigger a reveal
			
			if (!tiles.at(tileKey).IsRevealed())
			{
				tiles.at(tileKey).Reveal();
				//todo: recursive reveal, game win/fail
			}
		}
	}
	else
	{
		// find the menu buttons
	}
}

void Board::Render(sf::RenderWindow& window)
{
	map<pair<int, int>, Tile>::iterator it;
	for (it = tiles.begin(); it != tiles.end(); it++)
	{
		it->second.Render(window);
	}

	sf::Sprite counterSprite(TextureManager::GetTexture("digits"));
	counterSprite.setPosition(0, menuYPosition);
	window.draw(counterSprite);

	int menuXPos = (GetWidth() / 2) - 32;
	sf::Sprite faceTileSprite(TextureManager::GetTexture("face_happy"));
	faceTileSprite.setPosition(menuXPos, menuYPosition);
	window.draw(faceTileSprite);

	menuXPos += 96; // 32 pixels for rest of face button, plus 64 pixels of space
	sf::Sprite debugSprite(TextureManager::GetTexture("debug"));
	debugSprite.setPosition(menuXPos, menuYPosition);
	window.draw(debugSprite);

	for (unsigned int i = 1; i <= 3; i++)
	{
		menuXPos += 64; // width of each button
		sf::Sprite testSprite(TextureManager::GetTexture("test_" + std::to_string(i)));
		testSprite.setPosition(menuXPos, menuYPosition);
		window.draw(testSprite);
	}
}




