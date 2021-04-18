#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Board.h" 

using namespace std;
using namespace sf;

int main()
{
    TextureManager::SetImagesDirectory("images/");
    Board board;
    int boardWidth = board.GetWidth();
    int boardHeight = board.GetHeight();
    sf::RenderWindow window(sf::VideoMode(boardWidth, boardHeight), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    while (window.isOpen())
    {
        if (boardWidth != board.GetWidth() || boardHeight != board.GetHeight())
        {
            // we have loaded a config that changes the size of the screen/board
            // so we want to recreate the window
            boardWidth = board.GetWidth();
            boardHeight = board.GetHeight();
            window.create(sf::VideoMode(boardWidth, boardHeight), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                auto position = sf::Mouse::getPosition(window);
                board.OnClick(event.mouseButton);

                //cout << "Row: " << position.y / 32 << " Column:" << position.x / 32 << endl;
                //cout << "click: xPos: " << position.x << " yPos: " << position.y << endl; 
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
