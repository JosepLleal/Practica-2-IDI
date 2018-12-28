#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Entity.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Player : public j1Entity
{
public:

	j1Player(int x, int y, EntityType type);

	virtual ~j1Player();

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
	float dt_player;
	bool added = false;
	uint player_fx;

};

#endif // __j1PLAYER_H__
