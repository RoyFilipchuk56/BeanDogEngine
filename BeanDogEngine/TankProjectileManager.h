#pragma once
#include "iTankProjectile.h"

class TankProjectileManager
{
public:
	std::vector<iTankProjectile*> projectiles;

	std::vector<iTankProjectile*> GetList();
	void AddTankProjectile(iTankProjectile* shot);
	void RemoveProjectile(iTankProjectile* shot);
	void Update(float deltaTime);
};