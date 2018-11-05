#include "stdafx.h"

#include "JN_ProjectileController.h"


JN_ProjectileController::JN_ProjectileController(int _maxProjectiles)
{
	maxProjectiles = _maxProjectiles;
	projectiles    = new JN_Projectile[maxProjectiles];
	CreateInitialProjectiles();
}

JN_ProjectileController::~JN_ProjectileController()
{
	// Deallocate memory
	delete[] projectiles;
	projectiles = NULL;
}

void JN_ProjectileController::CreateInitialProjectiles()
{
	JN_Projectile p;

	for (int i = 0; i < maxProjectiles; i++)
	{
		// TIL - Make all constants static otherwise EVERYTHING breaks
		p = JN_Projectile();
		p.Init(JN_RGB{ 255, 0, 0 });
		projectiles[i] = p;
	}
}

bool JN_ProjectileController::Shoot(SDL_Rect sourceRect, SDL_Rect targetRect)
{
	int projectileIndex = GetAvailableProjectileIndex();

	if (projectileIndex >= 0)
		projectiles[projectileIndex].SetTarget(sourceRect, targetRect);

	return true;
}

bool JN_ProjectileController::ProjectileAvailable()
{
	for (int i = 0; i < maxProjectiles; i++)
	{
		if (!projectiles[i].visible)
			return true;
	}
	return false;
}

int JN_ProjectileController::GetAvailableProjectileIndex()
{
	if (!ProjectileAvailable())
		return -1;

	for (int i = 0; i < maxProjectiles; i++)
	{
		if (!projectiles[i].visible)
			return i;
	}

	return -1;
}

void JN_ProjectileController::Update()
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i].Update();
}

void JN_ProjectileController::LateUpdate()
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i].LateUpdate();
}

void JN_ProjectileController::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i].Render(renderer);
}