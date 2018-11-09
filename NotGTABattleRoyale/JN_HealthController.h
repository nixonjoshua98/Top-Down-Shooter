#ifndef JN_HEALTH_MANAGER_H
#define JN_HEALTH_MANAGER_H

class JN_HealthController
{
public:
	JN_HealthController();

	JN_HealthController(int initial);

	void TakeDamage(int dmg);
	int GetHealth();
	int GetDamageTaken();

private:
	int health         = 0;
	int damageTaken    = 0;
	int startingHealth = 0;
};




#endif // !JN_HEALTH_MANAGER_H
