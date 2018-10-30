
#include "stdafx.h"
#include "projectileController.h"

ProjectileController::ProjectileController(int _maxProjectiles)
{
	maxProjectiles = _maxProjectiles;
	projectiles    = new Projectile[maxProjectiles];
	CreateInitialProjectiles();
}

ProjectileController::~ProjectileController()
{
	// Deallocate memory
	delete[] projectiles;
	projectiles = NULL;
}

void ProjectileController::CreateInitialProjectiles()
{
	Projectile p;
	for (int i = 0; i < maxProjectiles; i++)
	{
		// TIL - Make all constants static otherwise EVERYTHING breaks
		p = Projectile();
		p.Init(RGB{ 155, 155, 155 });
		projectiles[i] = p;
	}
}

bool ProjectileController::Shoot(SDL_Rect sourceRect, SDL_Rect targetRect)
{
	int projectileIndex = GetAvailableProjectileIndex();

	if (projectileIndex == -1) { return false; }

	projectiles[projectileIndex].SetTarget(sourceRect, targetRect);

	return true;
}

bool ProjectileController::ProjectileAvailable()
{
	for (int i = 0; i < maxProjectiles; i++)
	{
		if (!projectiles[i].visible)
		{
			return true;
		}
	}
	return false;
}

int ProjectileController::GetAvailableProjectileIndex()
{
	if (!ProjectileAvailable()) { return -1; }

	for (int i = 0; i < maxProjectiles; i++)
	{
		if (!projectiles[i].visible)
		{
			return i;
		}
	}

	return -1;
}

void ProjectileController::Update()
{
	for (int i = 0; i < maxProjectiles; i++)
	{
		projectiles[i].Update();
	}
}

void ProjectileController::LateUpdate()
{
	for (int i = 0; i < maxProjectiles; i++)
	{
		projectiles[i].LateUpdate();
	}
}

void ProjectileController::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < maxProjectiles; i++)
	{
		projectiles[i].Render(renderer);
	}
}