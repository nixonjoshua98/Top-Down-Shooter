#ifndef JN_HEALTH_MANAGER_H
#define JN_HEALTH_MANAGER_H

#include "JN_Logging.h"

class JN_HealthController
{
public:
	JN_HealthController();


	void Init(int startingHealth, JN_Logging *log);
	void TakeDamage(int dmg);
	int GetHealth();
	int GetDamageTaken();
	void SetHealth(int i)
	{
		health = i;
	}

private:
	int health         = 0;
	int damageTaken    = 0;
	int startingHealth = 0;

	JN_Logging *logObj = NULL;
};




#endif // !JN_HEALTH_MANAGER_H
