#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using std::string;

class MenuButton
{
	int xPos;
	int yPos;
	int width;
	string texture;
public:
	MenuButton();
	MenuButton(int xPos, int yPos, int width, string texture);
	void SetTexture(string texture);
	void Render(sf::RenderWindow& window);
	bool MouseInBounds(int xMousePos, int yMousePos);
};

