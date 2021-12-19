#include "CommandGroup.h"

CommandGroup::CommandGroup(int repeatValue) : parallelCommandsDone(false), serialCommandsDone(false), startCalled(false), friendlyName("")
	, repeatValue(repeatValue), currentCycle(0)
{
}

CommandGroup::~CommandGroup()
{
	for (iCommand* c : serialCommands)
	{
		delete c;
	}
	for (iCommand* c : parallelCommands)
	{
		delete c;
	}
	for (iCommand* c : finishedCommands)
	{
		delete c;
	}
}

void CommandGroup::SetFriendlyName(std::string name)
{
	friendlyName = name;
}

std::string CommandGroup::GetFriendlyName()
{
	return friendlyName;
}

void CommandGroup::AddCommandSerial(iCommand* command)
{
	serialCommands.push_back(command);
}

void CommandGroup::AddCommandParallel(iCommand* command)
{
	parallelCommands.push_back(command);
}

unsigned int CommandGroup::ParallelCommandCount()
{
	return parallelCommands.size();
}

unsigned int CommandGroup::SerialCommandCount()
{
	return serialCommands.size();
}

void CommandGroup::Initialize(sMessage startData)
{
}

void CommandGroup::Start(sMessage startData)
{
	if (startCalled)
		return;

	// Call first serial command Start
	if (!serialCommands.empty())
	{
		sMessage msg;
		serialCommands[0]->Start(msg);
	}

	// Call all parallel commands Start
	for (iCommand* command : parallelCommands)
	{
		sMessage msg;
		command->Start(msg);
	}

	startCalled = true;
}

void CommandGroup::End(sMessage endData)
{
}

void CommandGroup::Update(float deltaTime)
{
	// Serial Commands
	if (!serialCommands.empty() && !serialCommandsDone)
	{
		iCommand* currentCommand = serialCommands[0];
		currentCommand->Update(deltaTime);

		if (currentCommand->IsDone())
		{
			sMessage endMessage;
			currentCommand->End(endMessage);
			finishedCommands.push_back(currentCommand);
			serialCommands.erase(serialCommands.begin());

			// Check for more commands
			if (!serialCommands.empty())
			{
				iCommand* newCommand = serialCommands[0];

				sMessage startMsg;
				newCommand->Start(startMsg);
			}
			else
			{
				serialCommandsDone = true;
			}
		}
	}
	else
	{
		serialCommandsDone = true;
	}

	// Parallel Commands
	bool anyRunning = false;
	int count = parallelCommands.size();
	for (int i = 0; i < count; i++)
	{
		iCommand* command = parallelCommands[i];

		if (!command->IsDone())
		{
			anyRunning = true;
			command->Update(deltaTime);
			// Might want to add some code to check if the parallel commands are finished and 
			//call their End function
		}
	}

	if (!anyRunning)
	{
		parallelCommandsDone = true;
	}
}

bool CommandGroup::IsDone(void)
{
	if (serialCommandsDone && parallelCommandsDone)
	{
		// -1 means repeat infinitely
		if (repeatValue == -1 || currentCycle < repeatValue)
		{
			for (int i = 0; i < finishedCommands.size(); i++)
			{
				serialCommands.push_back(finishedCommands[i]);
			}
			finishedCommands.clear();
			serialCommandsDone = false;
			parallelCommandsDone = false;

			// Reset each command by calling their start()
			sMessage msg;
			startCalled = false;
			Start(msg);

			currentCycle++;
		}
	}


	return (serialCommandsDone && parallelCommandsDone);
}

int CommandGroup::GetRepeatValue()
{
	return repeatValue;
}
