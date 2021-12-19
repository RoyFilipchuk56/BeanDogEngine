#include "MapManager.h"

MapManager::MapManager()
{
	LoadMap();
    PrintMap();
}

void MapManager::LoadMap()
{
    std::ifstream theFile(CONFIG_DIR + std::string("TheChambersOfGeribaiTheDeranged01.tsv"));
    std::string nextLine;

    if (theFile.is_open())
    {
        while (std::getline(theFile, nextLine))
        {
            std::vector<std::string> tempRow;
            for (int i = 0; i < nextLine.size(); i++)
            {
                if (nextLine.at(i) == 'F')
                {
                    tempRow.push_back("F");
                }
                else if (nextLine.at(i) == '-')
                {
                    tempRow.push_back("-");
                }
                else if (nextLine.at(i) == 'D')
                {
                    if (!(i + 1 <= nextLine.size()))
                    {
                        if (nextLine.at(i + 1) == 'P')
                        {
                            i++;
                            tempRow.push_back("DP");
                            continue;
                        }
                    }
                    tempRow.push_back("D");
                }
                else if(nextLine.at(i) == 'L')
                {
                    tempRow.push_back("L");
                }
                else if (nextLine.at(i) == 'U')
                {
                    tempRow.push_back("U");
                }
            }
            mapLayout.push_back(tempRow);
        }
        theFile.close();
    }
    //Reverse the rows (0,0 in array is 0,0 in world space. 1,0 in array is equal to y=1, x=0);
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

void MapManager::DrawMap(std::vector<cMesh*>& vecMesh, cVAOManager* vaoManager, unsigned int program)
{
    //load in all the dungeon models
    sModelDrawInfo Floor1info;
    if (!vaoManager->LoadModelIntoVAO(MODEL_DIR + std::string("SM_Env_Dwarf_Floor_02.ply"), Floor1info, program))
    {
        std::cout << "Error: " << "SM_Env_Dwarf_Floor_02.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "SM_Env_Dwarf_Floor_02.ply" << " loaded OK" << std::endl;
        std::cout << Floor1info.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << Floor1info.numberOfTriangles << " triangles loaded" << std::endl;
    }

    sModelDrawInfo Wall1info;
    if (!vaoManager->LoadModelIntoVAO(MODEL_DIR + std::string("SM_Env_Dwarf_Wall_06.ply"), Wall1info, program))
    {
        std::cout << "Error: " << "SM_Env_Dwarf_Wall_06.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "SM_Env_Dwarf_Wall_06.ply" << " loaded OK" << std::endl;
        std::cout << Wall1info.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << Wall1info.numberOfTriangles << " triangles loaded" << std::endl;
    }

    sModelDrawInfo DoorFrame1info;
    if (!vaoManager->LoadModelIntoVAO(MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Double_01.ply"), DoorFrame1info, program))
    {
        std::cout << "Error: " << "SM_Env_Dwarf_Wall_DoorFrame_Double_01.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "SM_Env_Dwarf_Wall_DoorFrame_Double_01.ply" << " loaded OK" << std::endl;
        std::cout << DoorFrame1info.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << DoorFrame1info.numberOfTriangles << " triangles loaded" << std::endl;
    }
    
    sModelDrawInfo DoorFrame2info;
    if (!vaoManager->LoadModelIntoVAO(MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Slider_01.ply"), DoorFrame2info, program))
    {
        std::cout << "Error: " << "SM_Env_Dwarf_Wall_DoorFrame_Slider_01.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "SM_Env_Dwarf_Wall_DoorFrame_Slider_01.ply" << " loaded OK" << std::endl;
        std::cout << DoorFrame2info.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << DoorFrame2info.numberOfTriangles << " triangles loaded" << std::endl;
    }

    sModelDrawInfo Door1info;
    if (!vaoManager->LoadModelIntoVAO(MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Double_01_Door_01.ply"), Door1info, program))
    {
        std::cout << "Error: " << "SM_Env_Dwarf_Wall_DoorFrame_Double_01_Door_01.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "SM_Env_Dwarf_Wall_DoorFrame_Double_01_Door_01.ply" << " loaded OK" << std::endl;
        std::cout << Door1info.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << Door1info.numberOfTriangles << " triangles loaded" << std::endl;
    }

    sModelDrawInfo Torchinfo;
    if (!vaoManager->LoadModelIntoVAO(MODEL_DIR + std::string("SM_Prop_Dwarf_Torch_01.ply"), Torchinfo, program))
    {
        std::cout << "Error: " << "SM_Prop_Dwarf_Torch_01.ply" << " Didn't load OK" << std::endl;
    }
    else
    {
        std::cout << "Good: " << "SM_Prop_Dwarf_Torch_01.ply" << " loaded OK" << std::endl;
        std::cout << Torchinfo.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << Torchinfo.numberOfTriangles << " triangles loaded" << std::endl;
    }

    int torchCounter = 2;
    bool areTorchesDrawn = false;
    //Row
    for (int i = 0; i < mapLayout.size(); i++)
    {

        //Column
        for (int j = 0; j < mapLayout[i].size(); j++)
        {
            //If theres a floor
            if (mapLayout[i][j] == "F")
            {
                //Create a mesh
                cMesh* floorMesh = new cMesh;
                floorMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Floor_02.ply");
                floorMesh->transformXYZ = glm::vec3(0, 0, 0);
                floorMesh->transformXYZ.x = i * 500.0f;
                floorMesh->transformXYZ.z = j * 500.0f;
                floorMesh->scale = 1;
                floorMesh->scaleXYZ = glm::vec3(1, 1, 1);
                floorMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                floorMesh->textureRatios[0] = 1.0f;
                vecMesh.push_back(floorMesh);
                //Add the entity top the actual list
                GameObject* obj = new GameObject(floorMesh);
                nGameObject::AddGameObject(obj);
                
                //Vector for what walls to draw
                std::vector<char> walls;
                
                if (j + 1 < mapLayout[i].size())
                {
                    if (mapLayout[i][j + 1] == "-")
                    {
                        walls.push_back('r');
                    }
                }
                if (j - 1 >= 0 && mapLayout[i][j - 1] == "-")
                {
                    walls.push_back('l');
                }
                if (i + 1 < mapLayout.size())
                {
                    if (mapLayout[i + 1][j] == "-")
                    {
                        walls.push_back('u');
                    }
                }
                if (i - 1 >= 0 && mapLayout[i-1][j] == "-")
                {
                    walls.push_back('d');
                }

                for (int index = 0; index < walls.size(); index++)
                {
                    //Create a mesh
                    cMesh* wallMesh = new cMesh;
                    wallMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Wall_06.ply");
                    wallMesh->transformXYZ = glm::vec3(0, 0, 0);
                    wallMesh->transformXYZ.x = floorMesh->transformXYZ.x;
                    wallMesh->transformXYZ.z = floorMesh->transformXYZ.z;
                    wallMesh->scale = 1;
                    wallMesh->scaleXYZ = glm::vec3(1, 1, 1);
                    wallMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                    wallMesh->textureRatios[0] = 1.0f;


                    if (walls[index] == 'u')
                    {
                        wallMesh->transformXYZ.z -= 500.0f;
                        wallMesh->rotationXYZ = glm::vec3(0, 1.5708f, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }

                    else if (walls[index] == 'd')
                    {
                        wallMesh->transformXYZ.x -= 500;
                        wallMesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }

                    else if (walls[index] == 'l')
                    {
                        wallMesh->transformXYZ.x -= 500.0f;
                        wallMesh->transformXYZ.z -= 500.0f;
                        wallMesh->rotationXYZ = glm::vec3(0, 3.14159, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }
                    
                    else if (walls[index] == 'r')
                    {
                        wallMesh->rotationXYZ = glm::vec3(0, 0, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }
                    vecMesh.push_back(wallMesh);
                    //Add the entity top the actual list
                    GameObject* obj = new GameObject(wallMesh);
                    nGameObject::AddGameObject(obj);

                    //Make the torches if the counter is set
                    if (torchCounter > 6)
                    {
                        //Create a mesh
                        cMesh* TorchMesh = new cMesh;
                        TorchMesh->meshName = MODEL_DIR + std::string("SM_Prop_Dwarf_Torch_01.ply");
                        TorchMesh->transformXYZ = glm::vec3(0, 0, 0);
                        TorchMesh->transformXYZ.x = floorMesh->transformXYZ.x;
                        TorchMesh->transformXYZ.z = floorMesh->transformXYZ.z;
                        TorchMesh->transformXYZ.y = 250.0f;
                        TorchMesh->scale = 1;
                        TorchMesh->scaleXYZ = glm::vec3(1, 1, 1);
                        TorchMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                        TorchMesh->textureRatios[0] = 1.0f;

                        
                        if (walls[index] == 'u')
                        {
                            TorchMesh->transformXYZ.z -= 250.0f;
                            TorchMesh->rotationXYZ = glm::vec3(0, 1.5708f, 0);
                            TorchMesh->rotationXYZQuat = quaternion::QuatFromAngles(TorchMesh->rotationXYZ);
                        }
                        
                        else if (walls[index] == 'd')
                        {
                            TorchMesh->transformXYZ.z -= 250;
                            TorchMesh->transformXYZ.x -= 500;
                            TorchMesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                            TorchMesh->rotationXYZQuat = quaternion::QuatFromAngles(TorchMesh->rotationXYZ);
                        }
                        
                        else if (walls[index] == 'l')
                        {
                            TorchMesh->transformXYZ.x -= 250.0f;
                            TorchMesh->transformXYZ.z -= 500.0f;
                            TorchMesh->rotationXYZ = glm::vec3(0, 3.14159, 0);
                            TorchMesh->rotationXYZQuat = quaternion::QuatFromAngles(TorchMesh->rotationXYZ);
                        }
                        
                        else if (walls[index] == 'r')
                        {
                            TorchMesh->transformXYZ.x -= 250.0f;
                            TorchMesh->rotationXYZ = glm::vec3(0, 0, 0);
                            TorchMesh->rotationXYZQuat = quaternion::QuatFromAngles(TorchMesh->rotationXYZ);
                        }
                        
                        vecMesh.push_back(TorchMesh);
                        //Add the entity top the actual list
                        //Torch* torchObj = new Torch(TorchMesh, walls[index]);
                        //nGameObject::AddTorch(torchObj);
                        areTorchesDrawn = true;
                    }
                }
            }

            //If theres a door
            if (mapLayout[i][j] == "D" || mapLayout[i][j] == "P")
            {
                //Create the floor mesh
                cMesh* floorMesh = new cMesh;
                if (mapLayout[i][j] == "D")
                {
                    floorMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Floor_02.ply");
                }
                else if (mapLayout[i][j] == "P")
                {
                    floorMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Slider_01.ply");
                }
                floorMesh->transformXYZ = glm::vec3(0, 0, 0);
                floorMesh->transformXYZ.x = i * 500.0f;
                floorMesh->transformXYZ.z = j * 500.0f;
                floorMesh->scale = 1;
                floorMesh->scaleXYZ = glm::vec3(1, 1, 1);
                floorMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                floorMesh->textureRatios[0] = 1.0f;
                vecMesh.push_back(floorMesh);
                //Add the entity top the actual list
                GameObject* floorobj = new GameObject(floorMesh);
                nGameObject::AddGameObject(floorobj);

                //Vector for what walls to draw
                std::vector<char> walls;

                if (j + 1 < mapLayout[i].size() && mapLayout[i][j + 1] == "-")
                {
                    walls.push_back('r');
                }
                if (j - 1 >= 0 && mapLayout[i][j - 1] == "-")
                {
                    walls.push_back('l');
                }
                if (i + 1 < mapLayout.size() && mapLayout[i + 1][j] == "-")
                {
                    walls.push_back('u');
                }
                if (i - 1 >= 0 && mapLayout[i - 1][j] == "-")
                {
                    walls.push_back('d');
                }

                //Draw the door
                if (walls.size() >= 1)
                {
                    //Create the door frame mesh
                    cMesh* doorFrameMesh = new cMesh;
                    doorFrameMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Double_01.ply");
                    doorFrameMesh->transformXYZ = glm::vec3(0, 0, 0);
                    doorFrameMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                    doorFrameMesh->textureRatios[0] = 1.0f;

                    //Door one mesh
                    cMesh* door1Mesh = new cMesh;
                    door1Mesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Double_01_Door_01.ply");
                    door1Mesh->transformXYZ = glm::vec3(0, 0, 0);
                    door1Mesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                    door1Mesh->textureRatios[0] = 1.0f;

                    //Door two mesh
                    cMesh* door2Mesh = new cMesh;
                    door2Mesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Wall_DoorFrame_Double_01_Door_01.ply");
                    door2Mesh->transformXYZ = glm::vec3(0, 0, 0);
                    door2Mesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                    door2Mesh->textureRatios[0] = 1.0f;

                    if (walls[0] == 'u' && walls[1] == 'd')
                    {
                        doorFrameMesh->transformXYZ.x = (i * 500.0f) - 500;
                        doorFrameMesh->transformXYZ.z = (j * 500.0f) - 250;

                        door1Mesh->transformXYZ.x = (i * 500.0f) - 375;
                        door1Mesh->transformXYZ.z = (j * 500.0f) - 250;
                        door1Mesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                        door1Mesh->rotationXYZQuat = quaternion::QuatFromAngles(door1Mesh->rotationXYZ);

                        door2Mesh->transformXYZ.x = (i * 500.0f) - 120;
                        door2Mesh->transformXYZ.z = (j * 500.0f) - 250;
                        door2Mesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                        door2Mesh->rotationXYZQuat = quaternion::QuatFromAngles(door2Mesh->rotationXYZ);
                    }
                    if (walls[0] == 'r' && walls[1] == 'l')
                    {
                        doorFrameMesh->transformXYZ.x = (i * 500.0f) - 250;
                        doorFrameMesh->transformXYZ.z = (j * 500.0f) - 500;
                        doorFrameMesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                        doorFrameMesh->rotationXYZQuat = quaternion::QuatFromAngles(doorFrameMesh->rotationXYZ);

                        door1Mesh->transformXYZ.x = (i * 500.0f) - 245;
                        door1Mesh->transformXYZ.z = (j * 500.0f) - 130;

                        door2Mesh->transformXYZ.x = (i * 500.0f) - 230;
                        door2Mesh->transformXYZ.z = (j * 500.0f) - 390;
                    }
                    //The door frame mesh
                    vecMesh.push_back(doorFrameMesh);
                    //Add the entity top the actual list
                    GameObject* doorFrameobj = new GameObject(doorFrameMesh);
                    nGameObject::AddGameObject(doorFrameobj);

                    //The door meshes
                    vecMesh.push_back(door1Mesh);
                    //Add the entity top the actual list
                    GameObject* door1obj = new GameObject(door1Mesh);
                    nGameObject::AddGameObject(door1obj);

                    vecMesh.push_back(door2Mesh);
                    //Add the entity top the actual list
                    GameObject* door2obj = new GameObject(door2Mesh);
                    nGameObject::AddGameObject(door2obj);
                }

                //Draw the walls
                for (int index = 0; index < walls.size(); index++)
                {
                    //Create a mesh
                    cMesh* wallMesh = new cMesh;
                    wallMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Wall_06.ply");
                    wallMesh->transformXYZ = glm::vec3(0, 0, 0);
                    wallMesh->transformXYZ.x = floorMesh->transformXYZ.x;
                    wallMesh->transformXYZ.z = floorMesh->transformXYZ.z;
                    wallMesh->scale = 1;
                    wallMesh->scaleXYZ = glm::vec3(1, 1, 1);
                    wallMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                    wallMesh->textureRatios[0] = 1.0f;
                    if (walls[index] == 'u')
                    {
                        wallMesh->transformXYZ.z -= 500.0f;
                        wallMesh->rotationXYZ = glm::vec3(0, 1.5708f, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }

                    else if (walls[index] == 'd')
                    {
                        wallMesh->transformXYZ.x -= 500;
                        wallMesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }

                    else if (walls[index] == 'l')
                    {
                        wallMesh->transformXYZ.x -= 500.0f;
                        wallMesh->transformXYZ.z -= 500.0f;
                        wallMesh->rotationXYZ = glm::vec3(0, 3.14159, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }

                    else if (walls[index] == 'r')
                    {
                        wallMesh->rotationXYZ = glm::vec3(0, 0, 0);
                        wallMesh->rotationXYZQuat = quaternion::QuatFromAngles(wallMesh->rotationXYZ);
                    }
                    vecMesh.push_back(wallMesh);
                    //Add the entity top the actual list
                    GameObject* obj = new GameObject(wallMesh);
                    nGameObject::AddGameObject(obj);
                }
            }
            if(mapLayout[i][j] == "U")
            {
                //figure out where the floor is
                std::vector<char> floor;
                if (j + 1 < mapLayout[i].size() && mapLayout[i][j + 1] == "F")
                {
                    floor.push_back('r');
                }
                else if (j - 1 >= 0 && mapLayout[i][j - 1] == "F")
                {
                    floor.push_back('l');
                }
                else if (i + 1 < mapLayout.size() && mapLayout[i + 1][j] == "F")
                {
                    floor.push_back('u');
                }
                else if (i - 1 >= 0 && mapLayout[i - 1][j] == "F")
                {
                    floor.push_back('d');
                }
                //Create the floor mesh
                cMesh* stairMesh = new cMesh;
                stairMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Stairs_01.ply");
                stairMesh->transformXYZ = glm::vec3(0, 0, 0);
                stairMesh->transformXYZ.x = i * 500.0f;
                stairMesh->transformXYZ.z = j * 500.0f;
                stairMesh->scale = 1;
                stairMesh->scaleXYZ = glm::vec3(1, 1, 1);
                stairMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                stairMesh->textureRatios[0] = 1.0f;

                if (floor[0] == 'u')
                {
                    stairMesh->transformXYZ.z -= 500.0f;
                    stairMesh->rotationXYZ = glm::vec3(0, 1.5708f, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                else if (floor[0] == 'd')
                {
                    stairMesh->transformXYZ.x -= 500;
                    stairMesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                else if (floor[0] == 'l')
                {
                    stairMesh->transformXYZ.z -= 500.0f;
                    stairMesh->rotationXYZ = glm::vec3(0, 3.14159, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                else if (floor[0] == 'r')
                {
                    stairMesh->rotationXYZ = glm::vec3(0, 0, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                vecMesh.push_back(stairMesh);
                //Add the entity top the actual list
                GameObject* stairobj = new GameObject(stairMesh);
                nGameObject::AddGameObject(stairobj);
            }
            if (mapLayout[i][j] == "L")
            {
                //figure out where the floor is
                std::vector<char> floor;
                if (j + 1 < mapLayout[i].size() && mapLayout[i][j + 1] == "F")
                {
                    floor.push_back('r');
                }
                else if (j - 1 >= 0 && mapLayout[i][j - 1] == "F")
                {
                    floor.push_back('l');
                }
                else if (i + 1 < mapLayout.size() && mapLayout[i + 1][j] == "F")
                {
                    floor.push_back('u');
                }
                else if (i - 1 >= 0 && mapLayout[i - 1][j] == "F")
                {
                    floor.push_back('d');
                }
                //Create the floor mesh
                cMesh* stairMesh = new cMesh;
                stairMesh->meshName = MODEL_DIR + std::string("SM_Env_Dwarf_Stairs_01.ply");
                stairMesh->transformXYZ = glm::vec3(0, 0, 0);
                stairMesh->transformXYZ.x = i * 500.0f;
                stairMesh->transformXYZ.y = -250.0f;
                stairMesh->transformXYZ.z = j * 500.0f;
                stairMesh->scale = 1;
                stairMesh->scaleXYZ = glm::vec3(1, 1, 1);
                stairMesh->textureNames[0] = "Dungeons_2_Texture_01_A.bmp";
                stairMesh->textureRatios[0] = 1.0f;

                if (floor[0] == 'u')
                {
                    stairMesh->transformXYZ.z -= 500.0f;
                    stairMesh->rotationXYZ = glm::vec3(0, -1.5708f, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                else if (floor[0] == 'd')
                {
                    stairMesh->transformXYZ.x -= 500;
                    stairMesh->rotationXYZ = glm::vec3(0, 1.5708f, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                else if (floor[0] == 'l')
                {
                    stairMesh->transformXYZ.x -= 500.0f;
                    stairMesh->rotationXYZ = glm::vec3(0, 0, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                else if (floor[0] == 'r')
                {
                    stairMesh->transformXYZ.z -= 500.0f;
                    stairMesh->rotationXYZ = glm::vec3(0, 0, 0);
                    stairMesh->rotationXYZQuat = quaternion::QuatFromAngles(stairMesh->rotationXYZ);
                }

                vecMesh.push_back(stairMesh);
                //Add the entity top the actual list
                GameObject* stairobj = new GameObject(stairMesh);
                nGameObject::AddGameObject(stairobj);
            }
            torchCounter++;
            if (areTorchesDrawn)
            {
                torchCounter = 0;
                areTorchesDrawn = false;
            }
        }
    }
}

std::vector<std::vector<std::string>> MapManager::GetMap()
{
    return mapLayout;
}