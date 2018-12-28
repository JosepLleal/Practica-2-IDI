#ifndef __j1FRUIT_H__
#define __j1FRUIT_H__

#include "j1Entity.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Fruit : public j1Entity
{
public:

	j1Fruit(int x, int y, EntityType type);

	virtual ~j1Fruit();

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
	float dt_fruit;

	uint fruit_fx; 

};

#endif // __j1FRUIT_H__