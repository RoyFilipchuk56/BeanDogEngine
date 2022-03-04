#pragma once
#include <string>
#include <vector>

#include "GlobalItems.h"
#include "Mediator.h"

#include "GameObject.h"
#include "TheCannon.h"

#define MAP_MAX_X 850
#define MAP_MIN_X -850
#define MAP_MAX_Y 580
#define MAP_MIN_Y -700

namespace nGameObject
{
	extern std::vector<GameObject*> gameObjects;
	
	extern int gameObjectCount;


	//Game objects
	void AddGameObject(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject);
	void UpdateGameObjects(float deltaTime);
	void DeleteGameObjects();
	//Misc
	GameObject* GetGameObjectByIndex(int index);
	GameObject* GetGameObjectByFriendlyName(std::string name);
	unsigned int GetGameObjectCount();
}