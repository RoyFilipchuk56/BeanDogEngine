#include "TriggerController.h"

TriggerController::~TriggerController()
{
}

void TriggerController::MakeTrigger(sMessage message)
{
	//Decode the message to make a trigger and a message
	Trigger theTrigger;
	//Dealing with the entity
	glm::vec3 position(message.vec_fData[0].x, message.vec_fData[0].y, message.vec_fData[0].z);
	GameObject* theEntity = new GameObject(position);
	//Set the triggers name
	theEntity->SetFriendlyName(message.vec_sData[0]);
	message.vec_sData.erase(message.vec_sData.begin());
	//Add the entity to the system
	nGameObject::AddGameObject(theEntity);
	theTrigger.myEntity = theEntity;
	//Get the radius
	theTrigger.radius = message.vec_fData[0].w;
	//Erase that part of the vector since were done with it
	message.vec_fData.erase(message.vec_fData.begin());

	//Get the whitelist size
	int whitelistSize = message.iData[0];
	//Erase that part of the vector since were done with it
	message.iData.erase(message.iData.begin());
	for (int i = 0; i < whitelistSize; i++)
	{
		//Copy the first element then remove it
		theTrigger.whitelist.push_back(message.vec_sData[0]);
		message.vec_sData.erase(message.vec_sData.begin());
	}
	//Get how many command groups are in the message
	theTrigger.commandGroupSize = message.iData[0];
	//Erase that part of the vector since were done with it
	message.iData.erase(message.iData.begin());
	theTrigger.commandGroups = message;
	triggers.push_back(theTrigger);
}

void TriggerController::update()
{
	//Make a message and if the responce is true then start sending it commands
	for (Trigger trigger : triggers)
	{
		sMessage responce;
		sMessage triggerMessage;
		triggerMessage.command = "Trigger Update";
		//used for easily getting the x, y, z
		glm::vec3 position = trigger.myEntity->GetPosition();
		triggerMessage.vec_fData.push_back(glm::vec4(position.x, position.y, position.z, trigger.radius));
		triggerMessage.iData.push_back(trigger.whitelist.size());
		for (int i = 0; i < trigger.whitelist.size(); i++)
		{
			triggerMessage.vec_sData.push_back(trigger.whitelist[i]);
		}
		mediator->RecieveMessage(triggerMessage, responce);

		//if theres a collision
		if (!responce.iData.empty() && responce.iData[0] == 1)
		{
			//start the fancy coding for making multiple messages from the commandGroups message
			//Make sure to check if friendly name is blank or not to add the name of the entity that hit it

			//Go through the command groups
			for (int i = 0; i < trigger.commandGroupSize; i++)
			{
				//Create a message to make a command group
				sMessage createCommand;
				createCommand.command = "Make Command Group";

				//Get the commandGroup name
				createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
				trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

				//Get the friendly name
				std::string friendlyName = trigger.commandGroups.vec_sData[0];
				trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
				//if the friendly name is blank then its the entity that triggered it
				if (friendlyName == "")
				{
					//TODO: Get the entity name from the responce
				}
				else
				{
					createCommand.vec_sData.push_back(friendlyName);
				}

				//get the repeat value
				createCommand.iData.push_back(trigger.commandGroups.iData[0]);
				trigger.commandGroups.iData.erase(trigger.commandGroups.iData.begin());

				//Get how many commands are in this group
				int commandsSize = trigger.commandGroups.iData[0];
				trigger.commandGroups.iData.erase(trigger.commandGroups.iData.begin());

				//Go through the commands and add them to the message
				for (int j = 0; j < commandsSize; j++)
				{
					//Get the name of the command
					std::string commandName = trigger.commandGroups.vec_sData[0];
					createCommand.vec_sData.push_back(commandName);
					trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

					if (commandName == "moveToLocation")
					{
						//Add the type(Serial or parallel)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
						//Add what were moving by (Seconds or speed)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

						//Add the value, x, y, z values
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
							trigger.commandGroups.vec_fData[0].y,
							trigger.commandGroups.vec_fData[0].z,
							trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());
					}

					else if (commandName == "moveToDistance")
					{
						//Add the type(Serial or parallel)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
						//Add what were moving by (Seconds or speed)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

						//Add the value, x, y, z values
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
																trigger.commandGroups.vec_fData[0].y,
																trigger.commandGroups.vec_fData[0].z,
																trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());
					}

					else if (commandName == "orientToLocation")
					{
						//Add the type(Serial or parallel)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
						//Add what were moving by (Seconds or speed)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
						//Add the value, x, y, z values
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
							trigger.commandGroups.vec_fData[0].y,
							trigger.commandGroups.vec_fData[0].z,
							trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());
						//Add weather or not its looking at a point or object
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
					}

					else if (commandName == "orientToRelative")
					{
						//Add the type(Serial or parallel)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
						//Add what were moving by (Seconds or speed)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

						//Add the value, x, y, z values
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
							trigger.commandGroups.vec_fData[0].y,
							trigger.commandGroups.vec_fData[0].z,
							trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());
					}

					else if (commandName == "followWaypoint")
					{
						//Add the type(Serial or parallel)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
						//Add what were moving by (Seconds or speed)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

						//Add the value, x, y, z values. x = value y = waypoint size
						float waypointSize = trigger.commandGroups.vec_fData[0].y;
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
							trigger.commandGroups.vec_fData[0].y,
							trigger.commandGroups.vec_fData[0].z,
							trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());
						for (int i = 0; i < waypointSize; i++)
						{
							createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
								trigger.commandGroups.vec_fData[0].y,
								trigger.commandGroups.vec_fData[0].z,
								trigger.commandGroups.vec_fData[0].w));
							trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());
						}
					}

					else if (commandName == "followObject")
					{
						//Add the type(Serial or parallel)
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());

						//Add the value, x, y, z values
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
							trigger.commandGroups.vec_fData[0].y,
							trigger.commandGroups.vec_fData[0].z,
							trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());

						//Add the value, x, y, z values Pt2
						createCommand.vec_fData.push_back(glm::vec4(trigger.commandGroups.vec_fData[0].x,
							trigger.commandGroups.vec_fData[0].y,
							trigger.commandGroups.vec_fData[0].z,
							trigger.commandGroups.vec_fData[0].w));
						trigger.commandGroups.vec_fData.erase(trigger.commandGroups.vec_fData.begin());

						//Add the Entity to follow
						createCommand.vec_sData.push_back(trigger.commandGroups.vec_sData[0]);
						trigger.commandGroups.vec_sData.erase(trigger.commandGroups.vec_sData.begin());
					}
				}
				mediator->RecieveMessage(createCommand);
			}
		}
	}
}

bool TriggerController::SetReciever(iMessage* receiver)
{
	mediator = receiver;
	return true;
}

bool TriggerController::RecieveMessage(sMessage message)
{
	if (message.command == "Make Trigger")
	{
		MakeTrigger(message);
	}
	return true;
}

bool TriggerController::RecieveMessage(sMessage message, sMessage& reply)
{
	return false;
}
