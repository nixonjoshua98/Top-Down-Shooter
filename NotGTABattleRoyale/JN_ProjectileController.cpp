#include "stdafx.h"

#include "JN_ProjectileController.h"
#include "JN_RealTimer.h"
#include "JN_WindowData.h"

#include <iostream>

// Default constructor
JN_ProjectileController::JN_ProjectileController()
{

}


// Init the controller
void JN_ProjectileController::Init(JN_Gameobject::Tag tag, int maxprojectiles, JN_Logging *logObj, JN_WindowData *windowData)
{
	this->tag = tag;
	this->windowData = windowData;
	this->logObj = logObj;
	this->maxProjectiles = maxprojectiles;
}


// De-constructor
JN_ProjectileController::~JN_ProjectileController()
{
	windowData = NULL;
	logObj = NULL;

	for (auto p : projectiles)
	{
		delete p;
		p = NULL;
	}
}


void JN_ProjectileController::CreateInitialProjectiles(SDL_Renderer *renderer)
{
	JN_Projectile *p;

	JN_RealTimer t = JN_RealTimer();

	for (int i = 0; i < maxProjectiles; i++)
	{
		p = new JN_Projectile();

		p->Init(tag, renderer, logObj, windowData);
		projectiles.push_back(p);
	}

	logObj->LogTimeSpan("Projectiles created", t.Tick());
}


bool JN_ProjectileController::Shoot(SDL_Rect sourceRect, SDL_Rect targetRect)
{
	int projectileIndex = GetAvailableProjectileIndex();

	if (projectileIndex >= 0)
		projectiles[projectileIndex]->SetTarget(sourceRect, targetRect);

	return projectileIndex >= 0;
}


bool JN_ProjectileController::ProjectileAvailable()
{
	for (int i = 0; i < maxProjectiles; i++)
	{
		if (!projectiles[i]->visible)
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
		if (!projectiles[i]->visible)
			return i;
	}

	return -1;
}

void JN_ProjectileController::Update()
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i]->Update();
}

void JN_ProjectileController::LateUpdate()
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i]->LateUpdate();
}

void JN_ProjectileController::Render(SDL_Renderer *renderer)
{
	for (int i = 0; i < maxProjectiles; i++)
		projectiles[i]->Render(renderer);
}