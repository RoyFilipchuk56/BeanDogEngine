#pragma once
#include "iMessage.h"
#include "GameObject.h"
#include "EntitySystem.h"
#include <vector>
#include <string>

struct Trigger
{
	//Have an entity so we can move it around in debug
	GameObject* myEntity;
	//Radius of the trigger
	float radius;
	//Names of entities that can enable the trigger
	std::vector<std::string> whitelist;
	//Names of entities that have already enabled them
	//TODO: Make it so its time based or they can enable after finishing the commands
	std::vector<std::string> blacklist;
	//The message to make the command groups
	sMessage commandGroups;
	int commandGroupSize;
};

class TriggerController : public iMessage
{
public:
	virtual ~TriggerController();

	void MakeTrigger(sMessage message);
	void update();

	virtual bool SetReciever(iMessage* receiver);
	virtual bool RecieveMessage(sMessage message);
	virtual bool RecieveMessage(sMessage message, sMessage& reply);

private:
	iMessage* mediator;
	std::vector<Trigger> triggers;
};