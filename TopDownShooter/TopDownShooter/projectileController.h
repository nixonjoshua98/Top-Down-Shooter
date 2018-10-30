#ifndef PROJECTILE_CONTROLLER_H
#define PROJECTILE_CONTROLLER_H

#include "projectile.h"
#include "SDL.h"

class ProjectileController
{
public:
	ProjectileController(int _maxProjectiles);
	~ProjectileController();

	bool Shoot(SDL_Rect sourceRecr, SDL_Rect targetRect);
	void Update();
	void LateUpdate();
	void Render(SDL_Renderer *renderer);

private:
	int maxProjectiles;
	Projectile *projectiles = NULL;

	void CreateInitialProjectiles();
	bool ProjectileAvailable();
	int GetAvailableProjectileIndex();
};


#endif // !PROJECTILE_CONTROLLER_H
