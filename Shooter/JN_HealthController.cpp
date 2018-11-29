#include "stdafx.h"

#include "JN_HealthController.h"
#include "JN_RealTimer.h"

JN_HealthController::JN_HealthController()
{

}

void JN_HealthController::Init(int startingHealth, JN_Logging *log)
{
	logObj = log;

	this->health = startingHealth;
	this->startingHealth = startingHealth;
}

void JN_HealthController::TakeDamage(int dmg)
{
	health -= dmg;
	damageTaken += dmg;

	health = health < 0 ? 0 : health;	// Minimum health set to 0 without needing to include <math.h>
}

int JN_HealthController::GetDamageTaken()
{
	return damageTaken;
}

int JN_HealthController::GetHealth()
{
	return health;
}