#ifndef __j1BOMB_H__
#define __j1BOMB_H__

#include "j1Entity.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Bomb : public j1Entity
{
public:

	j1Bomb(int x, int y, EntityType type);

	virtual ~j1Bomb();

	bool Start();
	bool Update(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Draw();

public:

	Animation idle;
	Animation dead;
	float dt_bomb;

	uint bomb_fx;

};

#endif // __j1BOMB_H__
