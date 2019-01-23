#include "stdafx.h"

#include "JN_Enemy.h"
#include "JN_ProjectileController.h"
#include "JN_RealTimer.h"

#include <iostream>



// Default constructor
JN_ProjectileController::JN_ProjectileController()
{

}


// Init the controller
void JN_ProjectileController::Init(JN_GameObject::Tag tag, int maxprojectiles, JN_Logging *logObj, JN_WindowData *windowData)
{
	this->tag = tag;
	this->windowData = windowData;
	this->logObj = logObj;
	this->maxProjectiles = maxprojectiles;
}


// De-constructor
JN_ProjectileController::~JN_ProjectileController()
{
	delete[] projectiles;

	windowData = NULL;
	logObj = NULL;
	projectiles = NULL;
}


void JN_ProjectileController::CreateInitialProjectiles()
{
	JN_Projectile p;

	JN_RealTimer t = JN_RealTimer();

	projectiles = new JN_Projectile[maxProjectiles];


	for (int i = 0; i < maxProjectiles; i++)
	{
		p = JN_Projectile();

		p.Init(tag, logObj, windowData);
		projectiles[i] = p;
	}

	logObj->LogTimeSpan("Projectiles created", t.Tick());
}


bool JN_ProjectileController::Shoot(SDL_Rect sourceRect, SDL_Rect targetRect)
{
	int projectileIndex = GetAvailableProjectileIndex();

	if (projectileIndex >= 0)
		projectiles[projectileIndex].SetTarget(sourceRect, targetRect);

	return projectileIndex >= 0;
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

void JN_ProjectileController::LateUpdate(std::vector<JN_Enemy*> &enemies)
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i].LateUpdate(enemies);
}

void JN_ProjectileController::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i].Render(renderer);
}


// Resize all projectiles 
void JN_ProjectileController::Resize(int xOffset, int yOffset)
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i].Resize(xOffset, yOffset);
}