#include "Tile.h"

Tile::Tile(float x, float y, bool hasMine)
{
	hiddenTile.setTexture(TextureManager::GetTexture("tile_hidden"));
	hiddenTile.setPosition(x, y);
	revealedTile.setTexture(TextureManager::GetTexture("tile_revealed"));
	revealedTile.setPosition(x, y);
	flag.setTexture(TextureManager::GetTexture("flag"));
	flag.setPosition(x, y);
	mineSprite.setTexture(TextureManager::GetTexture("mine"));
	mineSprite.setPosition(x, y);

	numAdjacentTiles = 0;
	this->x = x;
	this->y = y;
	tileValue = 0;

	this->hasMine = hasMine;
	isHidden = true;
	isFlagged = false;
}

sf::Sprite& Tile::getTileSprite()
{
	if (isHidden)
		return hiddenTile;
	else
		return revealedTile;
}

void Tile::flipTile()
{
	isHidden = false;
}

void Tile::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	hiddenTile.setPosition(x, y);
	revealedTile.setPosition(x, y);
}

void Tile::drawTile(sf::RenderWindow& window)
{
	window.draw(getTileSprite());

	if (!isHidden)
	{
		if (!hasMine)
		{
			if (tileValue != 0)
				window.draw(tileValueSprite);
		}
		else
		{
			window.draw(mineSprite);
		}
	}
	else if (isFlagged)
	{
		window.draw(flag);
	}
}

int Tile::setFlag()
{
	if (isHidden)
	{
		isFlagged = !isFlagged;
		if (isFlagged) { return 1; }
		else { return 0; }
	}
	return -1;
}

void Tile::setAdjacent(Tile* newTile)
{
	adjacentTiles.push_back(newTile);
}

void Tile::createTileValue()
{
	// calculate amount of mines adjacent
	for (unsigned int i = 0; i < adjacentTiles.size(); i++)
	{
		if ((adjacentTiles[i] != nullptr) && adjacentTiles[i]->hasMine)
			tileValue += 1;
	}
	// create the tile sprite if needed
	if (tileValue != 0)
	{
		string fileName = "number_" + to_string(tileValue);

		tileValueSprite.setTexture(TextureManager::GetTexture(fileName));
		tileValueSprite.setPosition(x, y);
	}
}

void Tile::clear()
{
	numAdjacentTiles = 0;
	hasMine = false;
	isFlagged = false;
	isHidden = true;
	tileValue = 0;

	adjacentTiles.clear();
}

void Tile::setMine(bool x)
{
	hasMine = x;
}

int Tile::getValue()
{
	return tileValue;
}

void Tile::recursiveReveal()
{
	if (!isHidden || isFlagged)
		return;

	flipTile();

	if (tileValue != 0)
		return;
	// loop through the adjacent tiles
	for (unsigned int i = 0; i < adjacentTiles.size(); i++)
	{
		// if the adjacent tile is on the board
		if (adjacentTiles[i] != nullptr)
		{
			// call the function on that tile
			adjacentTiles[i]->recursiveReveal();
		}
	}
}