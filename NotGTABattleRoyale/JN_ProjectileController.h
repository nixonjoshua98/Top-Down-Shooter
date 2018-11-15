#ifndef JN_PROJECTILE_CONTROLLER_H
#define JN_PROJECTILE_CONTROLLER_H

#include "JN_Projectile.h"
#include "JN_Logging.h"
#include "JN_WindowData.h"

#include "SDL.h"

#include <vector>

class JN_ProjectileController
{
public:
	JN_ProjectileController();
	~JN_ProjectileController();

	void Init(JN_Gameobject::Tag tag, int maxProjectiles, JN_Logging *logObj, JN_WindowData *windowData);

	bool Shoot(SDL_Rect sourceRecr, SDL_Rect targetRect);
	void Update();
	void LateUpdate();
	void Render(SDL_Renderer *renderer);

	void CreateInitialProjectiles(SDL_Renderer *renderer);

	JN_Gameobject::Tag tag;

private:
	JN_Logging *logObj = NULL;
	JN_WindowData *windowData = NULL;

	int maxProjectiles;
	std::vector<JN_Projectile*> projectiles;

	bool ProjectileAvailable();
	int GetAvailableProjectileIndex();
};


#endif // !JN_PROJECTILE_CONTROLLER_H
