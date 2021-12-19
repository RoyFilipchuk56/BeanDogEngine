#pragma once

#include "iMessage.h"

class iCommand
{
public:
	virtual ~iCommand() {};
	virtual void Initialize(sMessage startData) = 0;
	virtual void Start(sMessage startData) = 0;
	virtual void End(sMessage endData) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual bool IsDone(void) = 0;
};