#pragma once
#include "iMessage.h"
#include "EntitySystem.h"
#include "TriggerController.h"

//forward declaration
class TriggerController;

class Mediator : public iMessage
{
public:
	Mediator();
	virtual ~Mediator();

	void SetCommandFactory(iMessage* factory);
	void SetTriggerController(TriggerController* triggerController);
	// From iMessage
	virtual bool SetReciever(iMessage* receiver);
	virtual bool RecieveMessage(sMessage message);
	virtual bool RecieveMessage(sMessage message, sMessage& reply);


private:
	iMessage* commandFactory;
	TriggerController* triggerController;
};
