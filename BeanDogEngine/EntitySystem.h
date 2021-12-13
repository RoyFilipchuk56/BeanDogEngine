#pragma once
#include <string>
#include <vector>

#include "GameObject.h"

namespace nGameObject
{
	extern std::vector<GameObject*> gameObjects;
	extern int gameObjectCount;

	void AddGameObject(GameObject* gameObject);
	void RemoveGameObject(GameObject* gameObject);
	void UpdateGameObjects(float deltaTime);
	void DeleteGameObjects();
	GameObject* GetGameObjectByIndex(int index);
	GameObject* GetGameObjectByFriendlyName(std::string name);
	unsigned int GetGameObjectCount();
}