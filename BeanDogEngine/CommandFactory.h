#pragma once
#include "iMessage.h"


class CommandFactory : public iMessage
{
public:
	virtual ~CommandFactory();

	void MakeCommandGroup(sMessage message);


	virtual bool SetReciever(iMessage* receiver);
	virtual bool RecieveMessage(sMessage message);
	virtual bool RecieveMessage(sMessage message, sMessage& reply);

private:
	iMessage* mediator;
};

