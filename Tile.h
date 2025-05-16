#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Random.h"
#include "TextureManager.h"

class Tile
{
	sf::Sprite hiddenTile;
	sf::Sprite revealedTile;
	sf::Sprite tileValueSprite;
	sf::Sprite mineSprite;
	sf::Sprite flag;

	vector<Tile*> adjacentTiles;
	int numAdjacentTiles;
	bool isFlagged;
	float x, y;
	int tileValue;
public:
	bool hasMine;
	bool isHidden;

	Tile(float x = 0, float y = 0, bool hasMine = false);
	sf::Sprite& getTileSprite();
	void flipTile();
	void setPosition(float x, float y);
	void drawTile(sf::RenderWindow& window);
	bool tileFlagged() { return isFlagged; }
	int setFlag();
	void setAdjacent(Tile* newTile);
	void createTileValue();
	void clear();
	void setMine(bool x);
	int getValue();
	void recursiveReveal();
	bool tileHidden() { return isHidden; }
	bool& getIsFlagged() { return isFlagged; }
};