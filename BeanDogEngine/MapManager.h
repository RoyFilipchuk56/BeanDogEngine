#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class MapManager
{
public:
	std::vector<std::vector<char>> mapLayout;
	MapManager();
	void LoadMap();
	void PrintMap();
	std::vector<std::vector<char>> GetMap();
};