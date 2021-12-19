#include "CommandController.h"
#include <algorithm>

CommandController::CommandController() : needToCallStart(true), needToCallStartPermanent(true)
{
}

CommandController::~CommandController()
{
	for (int i = 0; i < commandGroups.size(); i++)
	{
		delete commandGroups[i];
	}
}

void CommandController::Update(float deltaTime)
{
	//Call regular commands
	if (!commandGroups.empty())
	{
		if (needToCallStart)
		{
			sMessage msg;
			commandGroups[0]->Start(msg);
			needToCallStart = false;
		}

		commandGroups[0]->Update(deltaTime);

		if (commandGroups[0]->IsDone())
		{
			// Delete finished command groups
			sMessage msg;
			commandGroups[0]->End(msg);
			delete commandGroups[0];
			std::vector<CommandGroup*>::iterator it;
			it = commandGroups.erase(commandGroups.begin());

			// Enter a new group
			if (!commandGroups.empty())
			{
				sMessage msg;
				commandGroups[0]->Start(msg);
			}
			else
			{
				needToCallStart = true;
			}
		}
	}

	//Call the permanent commands
	if (!permanentCommandGroups.empty())
	{
		for (CommandGroup* command : permanentCommandGroups)
		{
			if (needToCallStartPermanent)
			{
				sMessage msg;
				command->Start(msg);
				needToCallStartPermanent = false;
			}

			command->Update(deltaTime);

			if (command->IsDone())
			{

			}
		}
	}
}

void CommandController::AddCommandGroup(CommandGroup* commandGroup)
{
	//Check if command group is permanent
	if (commandGroup->GetRepeatValue() < 0)
	{
		permanentCommandGroups.push_back(commandGroup);
	}
	else
	{
		commandGroups.push_back(commandGroup);
	}
}
