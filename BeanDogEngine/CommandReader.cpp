#include "CommandReader.h"
#include <iostream>


CommandReader::CommandReader()
{
}

CommandReader::~CommandReader()
{
}


bool CommandReader::LoadCommandsFromFile(const std::string& filePath)
{
	using namespace rapidxml;

	// Open and parse xml file
	file<>* xmlFile = new file<>(filePath.c_str());
	xml_document<>* doc = new xml_document<>;
	doc->parse<0>(xmlFile->data());
	xml_node<>* root = doc->first_node("Cutscene");

	//parse the command groups
	bool result = ParseCommandGroups(root->first_node("CommandGroups"));

	result &= ParseTriggers(root->first_node("Triggers"));
	
	delete xmlFile;
	delete doc;

	return result;
}

//This command group is for on read loads
bool CommandReader::ParseCommandGroups(rapidxml::xml_node<>* valueIn)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	//Send iMessage for each command group
	/*	Message format	|	CommandGroup
		vec<string>		|	Command Group Name[0] - Friendly Name[1] - Command Name[2] - 
						|	Command Type[3] - Command Name other info(Measurement, object to follow, ect)
		vec<vec4>		|	Command Name data(xyz, value, ect)
		vec<int>		|	Repeat value
	*/	
	for (rapidxml::xml_node<>* child = valueIn->first_node("CommandGroup"); child; child = child->next_sibling())
	{
		//The message for the command group
		sMessage tempMsg;

		//Set the command name for the message
		tempMsg.command = "Make Command Group";

		//parse name
		result &= SetMessageString(child->first_attribute("name"), tempMsg);
		//parse friendlyName
		result &= SetMessageString(child->first_attribute("friendlyName"), tempMsg);
		//parse repeat
		result &= SetMessageInt(child->first_attribute("isRepeat"), tempMsg);

		//Parse each command
		result &= ParseCommands(child, tempMsg);

		if (result == false)
		{
			std::cout << "Error reading in command group " << std::endl;
			return false;
		}

		//Send command to mediator
		receiver->RecieveMessage(tempMsg);
	}

	return result;
}

//This command group is for triggers
bool CommandReader::ParseCommandGroups(rapidxml::xml_node<>* valueIn, sMessage& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;
	//Send iMessage for each command group
	/*	Message format	|	CommandGroup
		vec<string>		|	Command Group Name[0] - Friendly Name[1] - Command Name[2] -
						|	Command Type[3] - Command Name other info(Measurement, object to follow, ect)
		vec<vec4>		|	Command Name data(xyz, value, ect)
		vec<int>		|	Repeat value
	*/
	for (rapidxml::xml_node<>* child = valueIn->first_node("CommandGroup"); child; child = child->next_sibling())
	{
		//parse name
		result &= SetMessageString(child->first_attribute("name"), valueOut);
		//Set a friendly name but make it blank
		result &= SetMessageString(child->first_attribute("friendlyName"), valueOut);
		//parse repeat
		result &= SetMessageInt(child->first_attribute("isRepeat"), valueOut);

		//Check how many commands in this command group
		int commandSize = 0;
		for (rapidxml::xml_node<>* child2 = child->first_node("Command"); child2; child2 = child2->next_sibling())
		{
			commandSize++;
		}

		valueOut.iData.push_back(commandSize);

		//Parse each command
		result &= ParseCommands(child, valueOut);

		if (result == false)
		{
			std::cout << "Error reading in command group " << std::endl;
			return false;
		}
	}

	return result;
}

bool CommandReader::ParseCommands(rapidxml::xml_node<>* valueIn, sMessage& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	bool result = true;

	for (rapidxml::xml_node<>* child = valueIn->first_node("Command"); child; child = child->next_sibling())
	{
		std::string commandName;
		result &= SetValue(child->first_attribute("name"), commandName);
		valueOut.vec_sData.push_back(commandName);
		result &= SetMessageString(child->first_attribute("type"), valueOut);
		
		//Depending on the command, parse for different things
		//The move to command
		if (commandName == "moveToLocation" || commandName == "moveToDistance")
		{

			result &= SetMessageString(child->first_attribute("measurement"), valueOut);
			
			float x;
			float y;
			float z;
			float w;
			result &= SetValue(child->first_attribute("value"), x);
			result &= SetValue(child->first_attribute("x"), y);
			result &= SetValue(child->first_attribute("y"), z);
			result &= SetValue(child->first_attribute("z"), w);
			glm::vec4 tempVec(x, y, z, w);
			valueOut.vec_fData.push_back(tempVec);
		}
		//Orient to by disance command
		else if (commandName == "orientToRelative")
		{
			result &= SetMessageString(child->first_attribute("measurement"), valueOut);

			float x;
			float y;
			float z;
			float w;
			result &= SetValue(child->first_attribute("value"), x);
			result &= SetValue(child->first_attribute("x"), y);
			result &= SetValue(child->first_attribute("y"), z);
			result &= SetValue(child->first_attribute("z"), w);
			glm::vec4 tempVec(x, y, z, w);
			valueOut.vec_fData.push_back(tempVec);
		}
		//Orient to other object or point in space
		//TODO: maybe add a amount of time to stay looking at for
		else if (commandName == "orientToLocation")
		{
			result &= SetMessageString(child->first_attribute("measurement"), valueOut);

			float x;
			result &= SetValue(child->first_attribute("value"), x);
			
			//What are we looking at
			std::string lookingAt;
			result &= SetValue(child->first_attribute("lookingAt"), lookingAt);
			valueOut.vec_sData.push_back(lookingAt);
			if (lookingAt == "object")
			{
				glm::vec4 tempVec(x, 0, 0, 0);
				valueOut.vec_fData.push_back(tempVec);
				result &= SetMessageString(child->first_attribute("objectName"), valueOut);
			}
			else if (lookingAt == "point")
			{
				float y;
				float z;
				float w;
				result &= SetValue(child->first_attribute("x"), y);
				result &= SetValue(child->first_attribute("y"), z);
				result &= SetValue(child->first_attribute("z"), w);
				glm::vec4 tempVec(x, y, z, w);
				valueOut.vec_fData.push_back(tempVec);
			}
		}
		else if (commandName == "followWaypoint")
		{
			result &= SetMessageString(child->first_attribute("measurement"), valueOut);

			float x;
			float y = 0;
			result &= SetValue(child->first_attribute("value"), x);
			
			//Count how many waypoints are in it
			for (rapidxml::xml_node<>* child2 = child->first_node("Waypoint"); child2; child2 = child2->next_sibling())
			{
				y++;
			}
			//Push this data into the vector
			glm::vec4 tempVec(x, y, 0, 0);
			valueOut.vec_fData.push_back(tempVec);
			result &= ParseWaypoints(child, valueOut);
		}
		else if (commandName == "followObject")
		{
			float distance;
			float x;
			float y;
			float z;
			float maxSpeed;
			float timeLimit;

			result &= SetMessageString(child->first_attribute("objectName"), valueOut);
			result &= SetValue(child->first_attribute("distance"), distance);
			result &= SetValue(child->first_attribute("offX"), x);
			result &= SetValue(child->first_attribute("offY"), y);
			result &= SetValue(child->first_attribute("offZ"), z);
			result &= SetValue(child->first_attribute("maxSpeed"), maxSpeed);
			result &= SetValue(child->first_attribute("timeLimit"), timeLimit);
			glm::vec4 tempVec1(distance, x, y, z);
			glm::vec4 tempVec2(maxSpeed, timeLimit, 0, 0);
			valueOut.vec_fData.push_back(tempVec1);
			valueOut.vec_fData.push_back(tempVec2);
		}
	}
	return result;
}

bool CommandReader::ParseWaypoints(rapidxml::xml_node<>* valueIn, sMessage& valueOut)
{
	bool result = true;

	for (rapidxml::xml_node<>* child = valueIn->first_node("Waypoint"); child; child = child->next_sibling())
	{
		float x;
		float y;
		float z;
		result &= SetValue(child->first_attribute("x"), x);
		result &= SetValue(child->first_attribute("y"), y);
		result &= SetValue(child->first_attribute("z"), z);
		glm::vec4 tempVec(x, y, z, 0);
		valueOut.vec_fData.push_back(tempVec);
	}
	return result;
}

bool CommandReader::ParseTriggers(rapidxml::xml_node<>* valueIn)
{
	bool result = true;

	for (rapidxml::xml_node<>* child = valueIn->first_node("Trigger"); child; child = child->next_sibling())
	{
		//The message for the command group
		sMessage tempMsg;

		//Set the command name for the message
		tempMsg.command = "Make Trigger";

		float x;
		float y;
		float z;
		float radius;
		result &= SetValue(child->first_attribute("x"), x);
		result &= SetValue(child->first_attribute("y"), y);
		result &= SetValue(child->first_attribute("z"), z);
		result &= SetValue(child->first_attribute("radius"), radius);
		glm::vec4 tempVec(x, y, z, radius);
		tempMsg.vec_fData.push_back(tempVec);
		result &= SetMessageString(child->first_attribute("friendlyName"), tempMsg);

		int whitelistCount = 0;
		//Count how many waypoints are in it
		for (rapidxml::xml_node<>* child2 = child->first_node("Whitelist")->first_node("Object"); child2; child2 = child2->next_sibling())
		{
			whitelistCount++;
		}
		tempMsg.iData.push_back(whitelistCount);

		result &= ParseWhitelist(child->first_node("Whitelist"), tempMsg);

		int commandGroupCount = 0;
		for (rapidxml::xml_node<>* child2 = child->first_node("CommandGroup"); child2; child2 = child2->next_sibling())
		{
			commandGroupCount++;
		}
		tempMsg.iData.push_back(commandGroupCount);

		result &= ParseCommandGroups(child, tempMsg);

		receiver->RecieveMessage(tempMsg);
	}

	return result;
}

bool CommandReader::ParseWhitelist(rapidxml::xml_node<>* valueIn, sMessage& valueOut)
{
	bool result = true;

	for (rapidxml::xml_node<>* child = valueIn->first_node("Object"); child; child = child->next_sibling())
	{
		SetMessageString(child->first_attribute("friendlyName"), valueOut);
	}

	return result;
}

//Attribute setters
bool CommandReader::SetMessageInt(rapidxml::xml_attribute<>* valueIn, sMessage& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included float
	int value;
	value = std::stof(valueIn->value());
	valueOut.iData.push_back(value);
	return true;
}

bool CommandReader::SetMessageString(rapidxml::xml_attribute<>* valueIn, sMessage& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to string
	std::string value;
	value = valueIn->value();
	valueOut.vec_sData.push_back(value);
	return true;
}

bool CommandReader::SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included float
	valueOut = std::stof(valueIn->value());
	return true;
}

bool CommandReader::SetValue(rapidxml::xml_attribute<>* valueIn, int& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included float
	valueOut = std::stoi(valueIn->value());
	return true;
}

bool CommandReader::SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		return false;
	}

	//convert the attribute to the included string
	valueOut = valueIn->value();
	return true;
}


//Node setters
bool CommandReader::SetValueString(rapidxml::xml_node<>* valueIn, sMessage& valueOut)
{
	return false;
}


bool CommandReader::SetReciever(iMessage* receiver)
{
	this->receiver = receiver;
	return true;
}

bool CommandReader::RecieveMessage(sMessage message)
{
	return false;
}

bool CommandReader::RecieveMessage(sMessage message, sMessage& reply)
{
	return false;
}
