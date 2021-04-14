#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

void setBoard(sf::RenderWindow& window);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test board");// DO NOT DO THIS
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

        window.clear();
        setBoard(window);
        window.display();
    }
    return 0;
}

//TODO: this is just an example, reimplement this
void setBoard(sf::RenderWindow& window)
{
    Texture backgroundTile;
    backgroundTile.loadFromFile("images/tile_revealed.png");
    Sprite backgroundTileSprite(backgroundTile);
    Texture coverTile;
    coverTile.loadFromFile("images/tile_hidden.png");
    Sprite coverTileSprite(coverTile);

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            backgroundTileSprite.setPosition(j * 32, i * 32);
            window.draw(backgroundTileSprite);
            coverTileSprite.setPosition(j * 32, i * 32);
            window.draw(coverTileSprite);
        }
    }
}
