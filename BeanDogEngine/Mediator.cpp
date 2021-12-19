#include "Mediator.h"
#include <iostream>

Mediator::Mediator()
{
}

Mediator::~Mediator()
{
}


void Mediator::SetTriggerController(TriggerController* triggerController)
{
	this->triggerController = triggerController;
}


void Mediator::SetCommandFactory(iMessage* factory)
{
	commandFactory = factory;
}


bool Mediator::SetReciever(iMessage* receiver)
{

	return false;
}

bool Mediator::RecieveMessage(sMessage message)
{
	if (message.vec_sData.empty())
		return false;

	if (message.command == "Make Command Group")
	{
		commandFactory->RecieveMessage(message);
	}

	if (message.command == "Make Trigger")
	{
		triggerController->RecieveMessage(message);
	}

	return true;
}

bool Mediator::RecieveMessage(sMessage message, sMessage& reply)
{
	//Collision check the sent trigger and send back a reply 
	if (message.command == "Trigger Update")
	{
		//If theres no trigger data then return
		if (message.vec_fData.size() <= 0)
		{
			return false;
		}

		//Get the trigger location
		glm::vec3 triggerXYZ(message.vec_fData[0].x, message.vec_fData[0].y, message.vec_fData[0].z);
		//Get the trigger radius
		float radius = message.vec_fData[0].w;
		//Get the entity list
		std::vector<GameObject*> entitys = nGameObject::gameObjects;
		for (GameObject* entity : entitys)
		{
			bool isOnWhitelist = false;
			//Go through the whitelist
			for (int i = 0; i < message.iData[0]; i++)
			{
				//If the entity is on the whitelist
				if (entity->GetFriendlyName() == message.vec_sData[i])
				{
					isOnWhitelist = true;
				}
			}
			//If it isnt on the whitelist skip to the next entity
			if (!isOnWhitelist)
			{
				continue;
			}

			glm::vec3 midline = triggerXYZ - entity->GetPosition();
			float mag = glm::length(midline);

			std::cout << mag << std::endl;
			std::cout << triggerXYZ.x << " " << triggerXYZ.y << " " << triggerXYZ.z << std::endl;
			std::cout << entity->GetPosition().x << " " << entity->GetPosition().y << " " << entity->GetPosition().z << std::endl;

			//we we aint colliding
			if (mag <= 0.0f || mag >= radius + 0.5)
			{
				continue;
			}
			reply.iData.push_back(1);
			return true;
		}
	}
	reply.iData.push_back(0);
	return false;
}
