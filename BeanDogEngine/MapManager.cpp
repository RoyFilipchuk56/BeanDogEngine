#include "MapManager.h"

MapManager::MapManager()
{
	LoadMap();
    PrintMap();
}

void MapManager::LoadMap()
{
    std::ifstream theFile(CONFIG_DIR + std::string("Maze_827513.txt"));
    std::string nextLine;
    //    std::ifstream theFile("GalacticaOriginal_ASCII_no_text.ply");
        //    std::ifstream theFile( fileName.c_str() );

            // Did it open?
    if (theFile.is_open())          // theFile.is_open() == false
    {
        while (std::getline(theFile, nextLine))
        {
            std::vector<char> tempRow;
            for (int i = 0; i < nextLine.size(); i++)
            {
                if (nextLine.at(i) == 'X')
                {
                    tempRow.push_back('X');
                }
                else if (nextLine.at(i) == ' ')
                {
                    tempRow.push_back(' ');
                }
            }
            mapLayout.push_back(tempRow);
        }
        theFile.close();
    }
    //Reverse the rows so the tanks can just plug in their world space position (0,0 in array is 0,0 in world space. 1,0 in array is equal to y=1, x=0);
    std::reverse(mapLayout.begin(), mapLayout.end());
}

void MapManager::PrintMap()
{
    for (int i = 0; i < mapLayout.size(); i++)
    {
        for (int j = 0; j < mapLayout[i].size(); j++)
        {
            std::cout << mapLayout[i][j];
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<char>> MapManager::GetMap()
{
    return mapLayout;
}