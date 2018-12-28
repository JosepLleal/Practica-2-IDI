#include "j1App.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Fruit.h"
#include "j1Scene.h"

j1Fruit::j1Fruit(int x, int y, EntityType Type) : j1Entity(x, y, Type)
{
	current_animation = NULL;

	idle.LoadAnimations("coin", "idle");
	position.x = x;
	position.y = y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

j1Fruit::~j1Fruit() {}

bool j1Fruit::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/coin.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 28, 26 }, COLLIDER_FRUIT, App->entityManager);
	}

	fruit_fx = App->audio->LoadFx("audio/fx/get_coin.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Fruit::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Bat Update", Profiler::Color::Green);

	dt_fruit = dt;
	collider->SetPos(position.x , position.y );

	
	return true;
}

bool j1Fruit::PostUpdate()
{
	Draw();
	return true;
}

bool j1Fruit::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	
	return true;
}

void j1Fruit::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_fruit);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Fruit::OnCollision(Collider* c1, Collider* c2)
{
	
}

bool j1Fruit::Load(pugi::xml_node&)
{
	return true;
}
bool j1Fruit::Save(pugi::xml_node&) const
{
	return true;
}
