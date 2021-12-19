#include "EntitySystem.h"

namespace nGameObject
{
	std::vector<GameObject*> gameObjects;
	TheCannon* cannon = NULL;
	int gameObjectCount = 0;

	void nGameObject::AddGameObject(GameObject* gameObject)
	{
		//Add a gameObject to the global list and increase the count
		gameObjects.push_back(gameObject);
		gameObjectCount++;
	}

	void nGameObject::RemoveGameObject(GameObject* gameObject)
	{
		//Go through every game object and delete every gameObject that is the one sent in
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject), gameObjects.end());
		gameObjectCount--;
	}

	void nGameObject::UpdateGameObjects(float deltaTime)
	{
		//Update each gameobject and their commands
		for (int i = 0; i < gameObjectCount; i++)
		{
			gameObjects[i]->Update(deltaTime);
			gameObjects[i]->UpdateCommands(deltaTime);
		}
	}

	void nGameObject::DeleteGameObjects()
	{
		//delete game objects
		for (GameObject* obj : gameObjects)
		{
			delete obj;
		}
		//Make sure we cant call missing pointers
		gameObjects.clear();
		//reset the gameobject count
		gameObjectCount = 0;
	}

	GameObject* nGameObject::GetGameObjectByIndex(int index)
	{
		if (index >= gameObjectCount)
		{
			return nullptr;
		}

		return gameObjects[index];
	}

	GameObject* nGameObject::GetGameObjectByFriendlyName(std::string name)
	{
		for (GameObject* obj : gameObjects)
		{
			if (obj->GetFriendlyName() == name)
			{
				return obj;
			}
		}
		return nullptr;
	}

	unsigned int nGameObject::GetGameObjectCount()
	{
		return gameObjectCount;
	}
}
