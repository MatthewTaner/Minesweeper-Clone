#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <unordered_map>
#include "Random.h"
#include "TextureManager.h"
#include "Tile.h"
using namespace std;

class Board
{
	vector<vector<Tile>> board;
	vector<sf::Sprite> digits;

	int width;
	int height;
	void findAdjTiles(Tile& currentTile, int startRow, int startCol);
	int numMines;
	int minesLeft;
	bool gameLost;
	bool minesRevealed;
	bool gameWon;

public:
	struct Button
	{
		float btnX, btnY;
		sf::Sprite btnSprite;
		float btnWidth, btnHeight;


		function<void(void)> operation;

		Button() = default;
		Button(float btnX, float btnY, string textureName, function<void(void)> func);
		void drawButton(sf::RenderWindow& window);
	};

	vector<Button> buttons;
	Board(int height, int width, int numMines);
	void loadBoardFile(string fileName);
	void randomizeBoard();
	void clearBoard();
	void drawBoard(sf::RenderWindow& window);
	vector<vector<Tile>>& getBoard();
	void flipClickedTile(int x, int y);
	void flagTile(int x, int y);
	void revealMines();
	void revealMines2();
	void drawDigits(sf::RenderWindow& window);
};