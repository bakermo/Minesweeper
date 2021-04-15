// All credit for this class goes to Professor Fox
#include "TextureManager.h"

// "redeclare" the variable, so this file knows it exists and can use it
unordered_map<string, sf::Texture> TextureManager::textures;
string TextureManager::imagesDirectory;

void TextureManager::LoadTexture(string fileName)
{
	string path = imagesDirectory;
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	// if the texture doesn't exist...
	if (textures.find(textureName) == textures.end())
		LoadTexture(textureName);

	return textures[textureName];
}

void TextureManager::SetImagesDirectory(string directoryPath)
{
	imagesDirectory = directoryPath;
}

void TextureManager::Clear()
{
	textures.clear(); // get rid of all stored objects
}