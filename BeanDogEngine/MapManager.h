#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "EntitySystem.h"
#include "cVAOManager.h"

class MapManager
{
public:
	std::vector<std::vector<std::string>> mapLayout;
	MapManager();
	void LoadMap();
	void PrintMap();
	void DrawMap(std::vector<cMesh*>& vecMesh, cVAOManager* vaoManager, unsigned int program);
	std::vector<std::vector<std::string>> GetMap();
};