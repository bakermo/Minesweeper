#include "Board.h"

Board::Board()
{
	NewGame();
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
	boardConfig.mines = config[2];
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
		int mineCount = 0;
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
					mineCount++;
				}
				tiles.emplace(make_pair(currentColumn, currentRow), tile);
				currentColumn++;
			}
			currentColumn = 0;
			currentRow++;
		}
		this->columns = line.length();
		this->rows = currentRow++;
		this->mines = mineCount;
		menuYPosition = rows * tileSize;	
	}

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

void Board::NewGame()
{
	BoardConfig config = LoadFromConfig();
	this->columns = config.columns;
	this->rows = config.rows;
	this->mines = config.mines;

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
	InitializeMines();
	CreateMenu();
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

	// TODO: come back to this. it really isn't 
	// toggling cheat, we just want to reveal all
	// the mines and show them as depressed
	ToggleCheat();
	//map<pair<int, int>, Tile>::iterator it;
	//for (it = tiles.begin(); it != tiles.end(); it++)
	//{
	//	it->second.Render(window);
	//}
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
			Tile* tile = &tiles.at(tileKey);
			if (!tile->IsFlagged() && !tile->IsRevealed())
			{
				tile->Reveal();
				if (tile->HasMine())
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