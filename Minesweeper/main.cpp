#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include "Random.h";
#include "Board.h" // remove some of these that arent getting used
                    // todo: include leaker?

using namespace std;
using namespace sf;

Board LoadBoardFromConfig()
{
    ifstream file("boards/config.cfg");
    int config[3] = {0, 0, 0};
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

    Board board(config[0], config[1], config[2]);
    return board;
}

int main()
{
    TextureManager::SetImagesDirectory("images/");
    Board board = LoadBoardFromConfig();
    sf::RenderWindow window(sf::VideoMode(board.GetWidth(), board.GetHeight()), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    //int randomX = Random::Int(0, window.getSize().x);
    //int randomY = Random::Int(0, window.getSize().y);

    //cout << "random x" << randomX << endl;
    //cout << "random y" << randomY << endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cout << "right click" << endl;
                }
                // we don't want a simple "else" because there 
                // may be more than 2 mouse buttons that we don't 
                // want to "left click" on
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    cout << "left click" << endl;
                }
            }
        }

        window.clear(sf::Color::White);
        // let the board handle the rendering
        board.Render(window);
        window.display();
    }

    // Clear out any textures before the program ends
    TextureManager::Clear();
    return 0;
}
