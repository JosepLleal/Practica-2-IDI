#ifndef __j1PURPLE_FRUIT_H__
#define __j1PURPLE_FRUIT_H__

#include "j1Entity.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Purple_Fruit : public j1Entity
{
public:

	j1Purple_Fruit(int x, int y, EntityType type, int speed_y, int speed_x);

	virtual ~j1Purple_Fruit();

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
	float dt_fruit;

	uint fruit_fx;
	uint fall_fx;

};

#endif // __j1PURPLE_FRUIT_H__

