#pragma once
#include "iMediator.h"
#include "iTank.h"
#include "MapManager.h"
#include "GlobalItems.h"

class Mediator : public iMediator
{
public:
	Mediator();
	virtual ~Mediator();

	// From iMessage interface
	// Asyncronous
	virtual bool RecieveMessage(sMessage message);
	// Syncronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
	virtual bool SetReciever(iMessage* reciever);

	void setMap(std::vector<std::vector<char>> map);

private:
	std::vector<std::vector<char>> myMap;
};
