#include "Board.h"
Board::Board(int columns, int rows, int mines)
{
	this->columns = columns;
	this->rows = rows;
	this->mines = mines;
	menuYPosition = rows * tileSize;
	showMines = false;
	Reset();
}

void Board::Reset()
{
	gameOver = false;
	tiles.clear();
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < columns; col++)
		{
			Tile tile(col, row);
			// key the map by column, row
			tiles.emplace(make_pair(col, row), tile);
		}
	}

	InitializeMines();
	int menuButtonWidth = 64;
	int menuXPos = (GetWidth() / 2) - 32;
	gameBtn = MenuButton(menuXPos, menuYPosition, menuButtonWidth, "face_happy");

	menuXPos += (menuButtonWidth * 2); // space between game button and debug
	debugMineBtn = MenuButton(menuXPos, menuYPosition, menuButtonWidth, "debug");
	for (int i = 0; i < 3; i++)
	{
		menuXPos += menuButtonWidth;
		MenuButton btn = MenuButton(menuXPos, menuYPosition, menuButtonWidth, "test_" + std::to_string(i + 1));
		debugButtons[i] = btn;
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

void Board::InitializeMines()
{
	int minesPlaced = 0;
	while (minesPlaced < mines)
	{
		// rows and columns are 0 based
		int randomX = Random::Int(0, columns - 1);
		int randomY = Random::Int(0, rows - 1);
		pair<int, int> key = make_pair(randomX, randomY);
		if (!tiles.at(key).HasMine())
		{
			tiles.at(key).PlaceMine();
			minesPlaced++;
		}
	}
}

void Board::Win()
{
	gameOver = true;
	gameBtn.SetTexture("face_lose");
}

void Board::Lose()
{
	gameOver = true;
	gameBtn.SetTexture("face_lose");
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

int Board::GetHeight()
{
	return (rows * 32) + menuDepth;
}

int Board::GetWidth()
{
	return columns * 32;
}

void Board::OnClick(sf::Event::MouseButtonEvent mouseButtonEvent)
{
	if (!gameOver && mouseButtonEvent.y < menuYPosition)
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
				if (tiles.at(tileKey).HasMine())
				{
					Lose();
				}
				//todo: recursive reveal, game win/fail
			}
		}
	}
	else
	{
		if (gameBtn.MouseInBounds(mouseButtonEvent.x, mouseButtonEvent.y))
		{
			//Start a new game
			Reset();
		}
		else if (debugMineBtn.MouseInBounds(mouseButtonEvent.x, mouseButtonEvent.y))
		{
			ToggleCheat();
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (debugButtons[i].MouseInBounds(mouseButtonEvent.x, mouseButtonEvent.y))
				{
					// load the scenario and then break;
					break;
				}
			}
		}
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

	gameBtn.Render(window);
	debugMineBtn.Render(window);
	for (int i = 0; i < 3; i++)
	{
		debugButtons[i].Render(window);
	}
}

void Board::ToggleCheat()
{
	map<pair<int, int>, Tile>::iterator it;
	for (it = tiles.begin(); it != tiles.end(); it++)
	{
		it->second.ToggleCheat();
	}
}




