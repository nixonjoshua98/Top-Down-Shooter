#include "stdafx.h"

#include "JN_HealthController.h"

JN_HealthController::JN_HealthController()
{

}

JN_HealthController::JN_HealthController(int startingHealth)
{
	this->health = startingHealth;
	this->startingHealth = startingHealth;
}

void JN_HealthController::TakeDamage(int dmg)
{
	health -= dmg;
	damageTaken += dmg;

	// Minimum health set to 0 without needing to include <math.h>
	health = health < 0 ? 0 : health;
}

int JN_HealthController::GetDamageTaken()
{
	return damageTaken;
}

int JN_HealthController::GetHealth()
{
	return health;
}