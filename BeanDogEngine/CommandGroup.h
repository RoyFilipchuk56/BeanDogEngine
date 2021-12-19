#pragma once
#include "iCommand.h"
#include <vector>

class CommandGroup : public iCommand
{
public:
	CommandGroup(int repeatValue);
	virtual ~CommandGroup();
	void SetFriendlyName(std::string name);
	std::string GetFriendlyName();

	void AddCommandSerial(iCommand* command);
	void AddCommandParallel(iCommand* command);
	unsigned int ParallelCommandCount();
	unsigned int SerialCommandCount();

	virtual void Initialize(sMessage startData);
	virtual void Start(sMessage startData);
	virtual void End(sMessage endData);
	virtual void Update(float deltaTime);
	virtual bool IsDone(void);

	int GetRepeatValue();

private:
	std::vector <iCommand*> parallelCommands;
	std::vector <iCommand*> serialCommands;

	std::vector <iCommand*> finishedCommands;

	bool serialCommandsDone;
	bool parallelCommandsDone;
	bool startCalled;
	int repeatValue;
	int currentCycle;

	std::string friendlyName;
};

