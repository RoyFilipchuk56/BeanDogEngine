#pragma once

#include "CommandGroup.h"

class CommandController
{
public:
	CommandController();
	~CommandController();

	void Update(float deltaTime);
	void AddCommandGroup(CommandGroup* commandGroup);

private:
	std::vector <CommandGroup*> commandGroups;
	std::vector <CommandGroup*> permanentCommandGroups;
	bool needToCallStart;
	bool needToCallStartPermanent;
};

