#include "TankProjectileManager.h"
#include <algorithm>
std::vector<iTankProjectile*> TankProjectileManager::GetList()
{
	return projectiles;
}

void TankProjectileManager::AddTankProjectile(iTankProjectile* shot)
{
	projectiles.push_back(shot);
}

void TankProjectileManager::RemoveProjectile(iTankProjectile* shot)
{
	shot->SetIsAlive(false);
	projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), shot));
}

void TankProjectileManager::Update(float deltaTime)
{
	for (iTankProjectile* projectile : projectiles)
	{
		projectile->Update(deltaTime);
	}
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (!projectiles[i]->GetIsAlive())
		{
			RemoveProjectile(projectiles[i]);
			i--;
			continue;
		}
	}
}