#pragma once
#include "iTank.h"
#include "iMessage.h"
#include "iMediator.h"
#include "GlobalItems.h"

#include "PlayerTank.h"
#include "EnemyTank.h"

class TankFactory
{
private:
	iMessage* theMediator;

public:
	TankFactory();
	~TankFactory();

	void SetMediator(iMediator* mediator);

	iTank* CreateTank(sMessage typeOfTank);

	void SetUpTanks();
};