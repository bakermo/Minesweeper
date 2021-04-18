#include "Board.h"

Board::Board()
{
	NewGame();
}

void Board::NewGame()
{
	BoardConfig config = LoadFromConfig();
	this->columns = config.columns;
	this->rows = config.rows;
	this->mineCount = config.mineCount;
	this->tilesRevealed = 0;

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
	menuYPosition = rows * tileSize;
	RandomizeMines();
	MapAdjacentTiles();
	CreateMenu();
}


BoardConfig Board::LoadFromConfig()
{
	ifstream file("boards/config.cfg");
	int config[3] = { 0, 0, 0 };
	if (file.is_open())
	{
		string line;
		for (int i = 0; i < 3; i++)
		{
			getline(file, line);
			istringstream lineStream(line);
			config[i] = stoi(line);
		}
	}
	BoardConfig boardConfig;
	boardConfig.columns = config[0];
	boardConfig.rows = config[1];
	boardConfig.mineCount = config[2];
	return boardConfig;
}

void Board::LoadTestScenario(string board)
{
	gameOver = false;
	tiles.clear();
	string fileName = "boards/" + board + ".brd";
	ifstream file(fileName);
	if (file.is_open())
	{
		int currentColumn = 0;
		int currentRow = 0;
		int minesPlaced = 0;
		string line;
		while (getline(file, line))
		{
			istringstream lineStream(line);
			for (char c : line)
			{
				Tile tile(currentColumn, currentRow);
				if (c != '0')
				{
					tile.PlaceMine();
					minesPlaced++;
				}
				tiles.emplace(make_pair(currentColumn, currentRow), tile);
				currentColumn++;
			}
			currentColumn = 0;
			currentRow++;
		}
		this->columns = line.length();
		this->rows = currentRow++;
		this->mineCount = minesPlaced;
		this->tilesRevealed = 0;
		menuYPosition = rows * tileSize;	
	}

	MapAdjacentTiles();
	CreateMenu();
}

void Board::CreateMenu()
{
	int menuButtonWidth = 64;
	int menuXPos = (GetWidth() / 2) - tileSize;
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

void Board::RandomizeMines()
{
	int minesPlaced = 0;
	while (minesPlaced < mineCount)
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

pair<int, int> Board::GetAdjacentTileKey(int column, int row, int adjacentIndex)
{
	/* there can be at max 8 adjacent tiles
	   we will arbitrarily consider 0 index to be the top left corner
	   and counting clockwise until we reach middle left
	   ex: 0 1 2
		   7 X 3
		   6 5 4
	*/
	int adjacentColumn = -1;
	int adjacentRow = -1;
	// gross :/
	switch (adjacentIndex)
	{
		case 0 :
			adjacentRow = row - 1;			// up 1 row
			adjacentColumn = column - 1;	// left one column
			break;
		case 1: 
			adjacentRow = row - 1;			// up 1 row
			adjacentColumn = column;		// same column
			break;
		case 2:
			adjacentRow = row - 1;			// up 1 row
			adjacentColumn = column + 1;	// right 1 column
			break;
		case 3:
			adjacentRow = row;				// same row
			adjacentColumn = column + 1;	// right 1 column
			break;
		case 4:
			adjacentRow = row + 1;			// down 1 row
			adjacentColumn = column + 1;	// right 1 column
			break;
		case 5:
			adjacentRow = row + 1;			// down 1 row
			adjacentColumn = column;		// same column
			break;
		case 6:	
			adjacentRow = row + 1;			// down 1 row
			adjacentColumn = column - 1;	// left 1 column
			break;
		case 7:
			adjacentRow = row;				// same row
			adjacentColumn = column - 1;	// left 1 column
		default:
			break;
	}

	return make_pair(adjacentColumn, adjacentRow);
}

void Board::MapAdjacentTiles()
{
	// There is most certainly a way more obvious way to do this
	// with some kind of tree structure or maybe even using stacks,
	// but I've already started down this map road and have time 
	// constraints, so here we go...
	// I'll visit this some day for a total refactor.
	map<pair<int, int>, Tile>::iterator it;
	for (it = tiles.begin(); it != tiles.end(); it++)
	{
		vector<Tile*> adjacentTiles;
		
		int column = it->first.first;
		int row = it->first.second;

		// filthy obscene O(n^3). Hope you don't have a a ton of tiles!
		for (int i = 0; i < 8; i++)
		{
			auto key = GetAdjacentTileKey(column, row, i);
			// determine if tile exists at that position
			auto adj = tiles.find(key);
			if (adj != tiles.end())
			{
				// this adjacent tile exists
				Tile* adjTile = &tiles.at(key);
				adjacentTiles.push_back(adjTile);
			}
		}
		// map the adjacent tile pointers
		it->second.SetAdjacentTiles(adjacentTiles);
	}
}

void Board::CheckWin()
{
	if (tilesRevealed >= (tiles.size() - mineCount))
	{
		gameOver = true;
		gameBtn.SetTexture("face_win");

		// Flag all the mines
		map<pair<int, int>, Tile>::iterator it;
		for (it = tiles.begin(); it != tiles.end(); it++)
		{
			if (it->second.HasMine() && !it->second.IsFlagged())
				it->second.ToggleFlag();
		}
	}
}

void Board::Lose()
{
	gameOver = true;
	gameBtn.SetTexture("face_lose");

	// Reveal all the mines
	map<pair<int, int>, Tile>::iterator it;
	for (it = tiles.begin(); it != tiles.end(); it++)
	{
		if (it->second.HasMine()) 
			it->second.ShowMine();
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
	return mineCount;
}

int Board::GetHeight()
{
	return (rows * tileSize) + menuDepth;
}

int Board::GetWidth()
{
	return columns * tileSize;
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
			if (tiles.at(tileKey).IsFlagged())
				flagCount++;
			else
				flagCount--;
		}
		else if (mouseButtonEvent.button == sf::Mouse::Left)
		{
			// we explicitly "else if" here so that 
			// other mouse buttons don't trigger a reveal
			Tile* tile = &tiles.at(tileKey);
			if (!tile->IsFlagged() && !tile->IsRevealed())
			{
				tilesRevealed += tile->Reveal();
				if (tile->HasMine())
				{
					Lose();
				}
				else
				{
					CheckWin();
				}
			}
		}
	}
	else
	{
		if (gameBtn.MouseInBounds(mouseButtonEvent.x, mouseButtonEvent.y))
		{
			//Start a new game
			NewGame();
		}
		else if (!gameOver && debugMineBtn.MouseInBounds(mouseButtonEvent.x, mouseButtonEvent.y))
		{
			ToggleCheat();
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (debugButtons[i].MouseInBounds(mouseButtonEvent.x, mouseButtonEvent.y))
				{
					LoadTestScenario("testboard" + std::to_string(i + 1));
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