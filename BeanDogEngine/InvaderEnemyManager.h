#pragma once
#include "iMessage.h"
#include "EntitySystem.h"
#include "random_helpers.h"
#include "GlobalItems.h"

#define MAX_ROW_COUNT 10
#define MAX_COLUMN_COUNT 5
#define ENEMY_SPAWN_X_START 580
#define ENEMY_SPAWN_Y_START 500
#define ENEMY_X_OFFSET 130
#define ENEMY_Y_OFFSET 100
#define ENEMY_WIN_Y -350

class InvaderEnemyManager : public iMessage
{
public:
	InvaderEnemyManager();
	virtual ~InvaderEnemyManager();

	void update(float deltaTime);
	void MoveEnemies(float deltaTime);

	virtual bool SetReciever(iMessage* receiver);
	virtual bool RecieveMessage(sMessage message);
	virtual bool RecieveMessage(sMessage message, sMessage& reply);

private:
	iMessage* mediator;
	bool isMovingLeft;
	bool areEnemysMoving;
	float timeUntilNextMove;
	float timeSinceLastMove;
	float timeSinceLastColumbMove;
	int columnToMove;
};