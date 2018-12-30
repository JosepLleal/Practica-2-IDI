#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Red_Fruit.h"
#include "j1Yellow_Fruit.h"
#include "j1Blue_Fruit.h"
#include "j1Green_Fruit.h"
#include "j1Purple_Fruit.h"
#include "j1Bomb.h"
#include "j1Player.h"


#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Awaking Entity manager");

	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	p2List_item<j1Entity*>* iterator;
	for (iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Start();
	}
	
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManager Update", Profiler::Color::Green);

	bool ret = true;

	accumulated_time += dt;
	if (accumulated_time >= UpdateCycle)
		do_logic = true;

	p2List_item<j1Entity*>* iterator;

	for( iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Update(dt, do_logic);
	}

	if (do_logic) {
		accumulated_time = 0.0f;
		do_logic = false;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::Red);

	p2List_item<j1Entity*>* iterator;
		
	for ( iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<j1Entity*>* iterator;

	for ( iterator = entities.start; iterator != nullptr && ret == true; iterator = iterator->next)
	{
		iterator->data->CleanUp();
	}

	return ret;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	p2List_item<j1Entity*>* iterator;

	for ( iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->collider == c1)
		{
			iterator->data->OnCollision(c1, c2);
			break;
		}
	}
}

void j1EntityManager::CreateEntity(int x, int y, EntityType type, int speed_y, int speed_x)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case EntityType::RED_FRUIT:
		ret = new j1Red_Fruit(x, y, type, speed_y, speed_x);
		break;
	case EntityType::YELLOW_FRUIT:
		ret = new j1Yellow_Fruit(x, y, type, speed_y, speed_x);
		break;
	case EntityType::BLUE_FRUIT:
		ret = new j1Blue_Fruit(x, y, type, speed_y, speed_x);
		break;
	case EntityType::GREEN_FRUIT:
		ret = new j1Green_Fruit(x, y, type, speed_y, speed_x);
		break;
	case EntityType::PURPLE_FRUIT:
		ret = new j1Purple_Fruit(x, y, type, speed_y, speed_x);
		break;
	case EntityType::BOMB:
		ret = new j1Bomb(x, y, type, speed_y, speed_x);
		break;
	}
	if (ret != nullptr)
		entities.add(ret);
}


void j1EntityManager::DestroyAllEntities()
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next) 
	{
		if (iterator->data != player)
		{
			iterator->data->CleanUp();
			entities.del(iterator);
			RELEASE(iterator->data);

			LOG("deleting entity");
		}
		
	}

}
void j1EntityManager::DestroyThisEntity(j1Entity* entity)
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data == entity)
		{
			iterator->data->CleanUp();
			entities.del(iterator);
			RELEASE(iterator->data);
		}
	}
}

void j1EntityManager::AddPlayer()
{
	player = new j1Player(500, 300, PLAYER);
	entities.add(player);
}


bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	
	player->Save(data);

	return true;
}