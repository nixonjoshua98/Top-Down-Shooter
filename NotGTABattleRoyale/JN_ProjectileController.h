#ifndef JN_PROJECTILE_CONTROLLER_H
#define JN_PROJECTILE_CONTROLLER_H

#include "JN_Projectile.h"
#include "SDL.h"

class JN_ProjectileController
{
public:
	JN_ProjectileController(int _maxProjectiles);
	~JN_ProjectileController();

	bool Shoot(SDL_Rect sourceRecr, SDL_Rect targetRect);
	void Update();
	void LateUpdate();
	void Render(SDL_Renderer *renderer);

private:
	int maxProjectiles;
	JN_Projectile *projectiles = NULL;

	void CreateInitialProjectiles();
	bool ProjectileAvailable();
	int GetAvailableProjectileIndex();
};


#endif // !JN_PROJECTILE_CONTROLLER_H
