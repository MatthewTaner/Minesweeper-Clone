#include "Board.h"
#include <sstream>

Board::Board(int height, int width, int numMines)
{
	this->numMines = numMines;
	this->width = width;
	this->height = height;
	minesLeft = 0;
	gameLost = false;
	gameWon = false;
	minesRevealed = false;

	float boardCenterX = ((width * 32.0f) / 2) - 36;
	float boardBottomY = height * 32.0f;

	//##### Load the button sprites #####

	buttons.push_back(Button(boardCenterX, boardBottomY, "face_happy", [=](void)->void {randomizeBoard(); }));
	buttons.push_back(Button(boardCenterX + 120, boardBottomY, "debug", [=](void)->void {revealMines2(); }));
	buttons.push_back(Button(boardCenterX + 184, boardBottomY, "test_1", [=](void)->void {loadBoardFile("testboard1"); }));
	buttons.push_back(Button(boardCenterX + 248, boardBottomY, "test_2", [=](void)->void {loadBoardFile("testboard2"); }));
	buttons.push_back(Button(boardCenterX + 312, boardBottomY, "test_3", [=](void)->void {loadBoardFile("testboard3"); }));

	//##### Load the 2d vector with the tiles #####
	for (int i = 0; i < height; i++)
	{
		vector<Tile> innerVec;
		for (int j = 0; j < width; j++)
		{
			innerVec.push_back(Tile(j * 32, i * 32, false));
		}
		board.push_back(innerVec);
	}

	randomizeBoard();
}

//***** Button Constructor *****
Board::Button::Button(float btnX, float btnY, string textureName, function<void(void)> func)
{
	this->btnX = btnX;
	this->btnY = btnY;
	this->btnHeight = 64.0f;
	this->btnWidth = 64.0f;

	operation = func;

	btnSprite.setTexture(TextureManager::GetTexture(textureName));
	btnSprite.setPosition(btnX, btnY);
}


void Board::Button::drawButton(sf::RenderWindow& window)
{
	window.draw(btnSprite);
}

void Board::loadBoardFile(string fileName)
{
	clearBoard();
	string path = "boards/";
	path += fileName + ".brd";

	ifstream file(path);
	float row = 0;
	float col = 0;
	string currentLine;
	while (getline(file, currentLine))
	{
		istringstream stream(currentLine);

		char token;
		while (stream.get(token))
		{
			string tokenS = string(1, token);
			bool mine;
			if (stoi(tokenS) == 1)
			{
				mine = true;
				minesLeft += 1;
			}
			else
			{
				mine = false;
			}

			board[row][col].setMine(mine);
			col += 1;
		}
		width = int(col);
		col = 0;
		row += 1;
	}
	height = int(row);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			findAdjTiles(board[i][j], i - 1, j - 1);
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			board[i][j].createTileValue();
		}
	}
}

void Board::randomizeBoard()
{
	clearBoard();

	minesLeft = 0;
	while (minesLeft < numMines)
	{
		int mineX = Random::Int(0, width - 1);
		int mineY = Random::Int(0, height - 1);

		if (!(board[mineY][mineX].hasMine))
		{
			board[mineY][mineX].setMine(true);
			minesLeft += 1;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			findAdjTiles(board[i][j], i - 1, j - 1);
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			board[i][j].createTileValue();
		}
	}
}

void Board::clearBoard()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			board[i][j].clear();
		}
	}
	minesLeft = 0;
	gameLost = false;
	gameWon = false;
	minesRevealed = false;

	buttons[0].btnSprite.setTexture(TextureManager::GetTexture("face_happy"));
}

void Board::drawBoard(sf::RenderWindow& window)
{
	//##### draw digits #####
	drawDigits(window);

	//##### draw buttons #####
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i].drawButton(window);
	}

	//##### draw tiles #####
	for (unsigned int i = 0; i < board.size(); i++)
	{
		for (unsigned int j = 0; j < board[i].size(); j++)
		{
			board[i][j].drawTile(window);
		}
	}
}

vector<vector<Tile>>& Board::getBoard()
{
	return board;
}

//***** Flips the clicked title an checks for win or loss *****
void Board::flipClickedTile(int x, int y)
{
	//##### Check if any buttons were clicked #####
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		Button current = buttons[i];
		if ((x >= current.btnX) && (x <= current.btnX + current.btnWidth) &&
			(y >= current.btnY) && (y <= current.btnY + current.btnHeight))
		{
			current.operation();
		}
	}

	if (!gameLost)
	{
		if (x <= (width * 32) && y <= (height * 32))
		{
			int row = y / 32;
			int col = x / 32;

			if (board[row][col].hasMine && !board[row][col].tileFlagged())
			{
				// Execute some game over sequence
				revealMines();

				// stop allowing action on the game board
				gameLost = true;

				//change to dead smiley face
				buttons[0].btnSprite.setTexture(TextureManager::GetTexture("face_lose"));
			}
			else
			{
				if (board[row][col].getValue() == 0)
					board[row][col].recursiveReveal();
				else if (!board[row][col].tileFlagged())
					board[row][col].flipTile();

				// loop through and count the amount of revealed tiles
				gameWon = true;
				for (unsigned int i = 0; i < board.size(); i++)
				{
					for (unsigned int j = 0; j < board[i].size(); j++)
					{
						// check if the tile is flipped and doesn't have mine
						if (board[i][j].tileHidden() && !(board[i][j].hasMine))
						{
							gameWon = false;
						}
					}
				}

				// if all non mine tiles are flipped
				if (gameWon)
				{
					// execute some game won sequence
					buttons[0].btnSprite.setTexture(TextureManager::GetTexture("face_win"));

					// flag the rest of the mines
					for (unsigned int i = 0; i < board.size(); i++)
					{
						for (unsigned int j = 0; j < board[i].size(); j++)
						{
							if (board[i][j].hasMine && !board[i][j].getIsFlagged())
							{
								board[i][j].getIsFlagged() = true;
								minesLeft -= 1;
							}
						}
					}
				}
			}
		}
	}
} 

void Board::flagTile(int x, int y)
{
	if (!gameLost)
	{
		if (x <= (width * 32) && y <= (height * 32))
		{
			int row = y / 32;
			int col = x / 32;
			int setFlagVal = board[row][col].setFlag();

			if (setFlagVal == 1)
				minesLeft -= 1;
			else if (setFlagVal == 0)
				minesLeft += 1;
		}
	}
}

// ***** Load adjacent tiles into vector *****
void Board::findAdjTiles(Tile& currentTile, int startRow, int startCol)
{
	int endRow = startRow + 3;
	int endCol = startCol + 3;
	for (int i = startRow; i < endRow; i++)
	{
		for (int j = startCol; j < endCol; j++)
		{
			try
			{
				if (!(i == (startRow + 1) && j == (startCol + 1)))
				{
					currentTile.setAdjacent(&board.at(i).at(j));
				}
			}
			catch (const exception&)
			{
				currentTile.setAdjacent(nullptr);
			}
		}
	}
}

void Board::revealMines()
{
	for (unsigned int i = 0; i < board.size(); i++)
	{
		for (unsigned int j = 0; j < board[i].size(); j++)
		{
			if (board[i][j].hasMine)
				board[i][j].flipTile();
		}
	}
	minesRevealed = true;
}

void Board::revealMines2()
{
	if (gameWon || gameLost)
	{

	}
	else if (!minesRevealed)
		revealMines();
	else
	{
		for (unsigned int i = 0; i < board.size(); i++)
		{
			for (unsigned int j = 0; j < board[i].size(); j++)
			{
				if (board[i][j].hasMine)
					board[i][j].isHidden = true;
			}
		}
		minesRevealed = false;
	}
}



void Board::drawDigits(sf::RenderWindow& window)
{
	string minesLeftCopy = to_string(minesLeft);

	for (unsigned int i = 0; i < minesLeftCopy.size(); i++)
	{
		// substring and cast as int
		string digitString = minesLeftCopy.substr(i, 1);

		if (digitString == "-")
		{
			sf::IntRect rectangle(10 * 21, 0, 21, 32);
			sf::Sprite digit(TextureManager::GetTexture("digits"), rectangle);
			digit.setPosition(25 + i * 21, height * 32);
			window.draw(digit);
		}
		else
		{
			int digitInt = stoi(digitString);
			sf::IntRect rectangle(digitInt * 21, 0, 21, 32);
			sf::Sprite digit(TextureManager::GetTexture("digits"), rectangle);
			digit.setPosition(25 + i * 21, height * 32);
			window.draw(digit);
		}
	}
}