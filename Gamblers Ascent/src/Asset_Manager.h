#pragma once
#include "SDL_Handler.h"

class Asset_Manager {
public:
	//amount of assets
	static const int AssetCount = 2;

	//array to store the assets
	SDL_Texture* Assets[AssetCount]{};

public:
	//constructor
	Asset_Manager(std::string(&Assetlinks)[AssetCount], SDL_Renderer* renderer);

	//destructor
	~Asset_Manager();

	//loads all the assets
	void LoadAssets();

	//destroys all the assets
	void DestroyAssets();

private:
	//links to the assets
	std::string(&AssetLinks)[AssetCount];

	//renderer
	SDL_Renderer* m_renderer;

private:
	//loads a surface
	SDL_Texture* LoadTexture(std::string filename);

	//destroys a surface
	void DestroyTexture(SDL_Texture* texture);
};