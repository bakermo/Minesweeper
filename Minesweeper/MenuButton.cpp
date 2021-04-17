#include "MenuButton.h"
#include "TextureManager.h"

MenuButton::MenuButton()
{
	this->xPos = 0;
	this->yPos = 0;
	this->width = 0;
	this->texture = "";
}

MenuButton::MenuButton(int xPos, int yPos, int width, string texture)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->width = width;
	this->texture = texture;
}

void MenuButton::SetTexture(string texture)
{
	this->texture = texture;
}

void MenuButton::Render(sf::RenderWindow& window)
{
	sf::Sprite sprite(TextureManager::GetTexture(texture));
	sprite.setPosition(xPos, yPos);
	window.draw(sprite);
}

bool MenuButton::MouseInBounds(int xMousePos, int yMousePos)
{
	bool inXBounds = (xMousePos >= xPos && xMousePos <= (xPos + width));
	bool inYBounds = (yMousePos >= yPos && yMousePos <= (yPos + width));
	return inXBounds && inYBounds;
}