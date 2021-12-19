#pragma once
#include "iMessage.h"
#include <rapidxml/rapidxml.hpp>
#include "rapidxml/rapidxml_utils.hpp"

struct Command
{
	std::string name;
	std::string type;
	std::string measurement;
	float x;
	float y;
	float z;
};

class CommandReader : public iMessage
{
private:
	iMessage* receiver;
public:
	CommandReader();
	virtual ~CommandReader();

	//Load the xml file
	bool LoadCommandsFromFile(const std::string& filePath);
	//parse the CommandGroups node
	bool ParseCommandGroups(rapidxml::xml_node<>* valueIn);
	//parse the CommandGroups node for a trigger (Dont imidiatly send to the mediator)
	bool ParseCommandGroups(rapidxml::xml_node<>* valueIn, sMessage& valueOut);
	//parse the Command node
	bool ParseCommands(rapidxml::xml_node<>* valueIn, sMessage& valueOut);
	//parse the waypoint node
	bool ParseWaypoints(rapidxml::xml_node<>* valueIn, sMessage& valueOut);
	//parse the triggers
	bool ParseTriggers(rapidxml::xml_node<>* valueIn);
	//parse the whitelist
	bool ParseWhitelist(rapidxml::xml_node<>* valueIn, sMessage& valueOut);

	//Attribute Values
	//sets the value of value out to the attribute for a int
	bool SetMessageInt(rapidxml::xml_attribute<>* valueIn, sMessage& valueOut);
	//sets the value of value out to the attribute for a string
	bool SetMessageString(rapidxml::xml_attribute<>* valueIn, sMessage& valueOut);
	//sets the value of value out to the attribute for a float
	bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut);
	//sets the value of value out to the attribute for a int
	bool SetValue(rapidxml::xml_attribute<>* valueIn, int& valueOut);
	//sets the value of value out to the attribute for a string
	bool SetValue(rapidxml::xml_attribute<>* valueIn, std::string& valueOut);

	//Node values
	//sets the value of a node to a string
	bool SetValueString(rapidxml::xml_node<>* valueIn, sMessage& valueOut);

	//iMessage stuff
	virtual bool SetReciever(iMessage* receiver);
	virtual bool RecieveMessage(sMessage message);
	virtual bool RecieveMessage(sMessage message, sMessage& reply);
};