#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "Random.h"
#include "TextureManager.h"
#include "Tile.h"
#include "Board.h"
using namespace std;

int main()
{
    ifstream file("boards/config.cfg");
    if (!file.is_open())
        throw runtime_error("Could not open config.cfg");

    string wdth, hght, mines;
    getline(file, wdth);
    getline(file, hght);
    getline(file, mines);

    unsigned int width = stoi(wdth);
    unsigned int height = stoi(hght);
    unsigned int numMines = stoi(mines);

    cout << width << endl;
    cout << height << endl;
    cout << numMines << endl;

    sf::RenderWindow window(sf::VideoMode({ width * 32, (height * 32) + 100 }), "Minesweeper");

    Board board(height, width, numMines);
    //board.loadBoardFile("lots_o_mines");
    //board.revealMines();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    board.flipClickedTile(position.x, position.y);
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    board.flagTile(position.x, position.y);
                }
            }
        }

        window.clear();
        board.drawBoard(window);
        window.display();
    }

    TextureManager::Clear();
    return 0;
}