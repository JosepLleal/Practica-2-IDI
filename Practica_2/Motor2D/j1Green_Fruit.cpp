#include "j1App.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Green_Fruit.h"
#include "j1Scene.h"
#include "j1Input.h"

j1Green_Fruit::j1Green_Fruit(int x, int y, EntityType Type, int speed_y, int speed_x) : j1Entity(x, y, Type, speed_y, speed_x)
{
	current_animation = NULL;

	idle.LoadAnimations("green_fruit", "idle");
	dead.LoadAnimations("green_fruit", "dead");
	position.x = x;
	position.y = y;

	speed.x = speed_x;
	speed.y = speed_y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

j1Green_Fruit::~j1Green_Fruit() {}

bool j1Green_Fruit::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/spritesheet.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 65, 65 }, COLLIDER_FRUIT, App->entityManager);
	}

	fruit_fx = App->audio->LoadFx("audio/fx/squashing.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Green_Fruit::Update(float dt, bool do_logic)
{

	dt_fruit = dt;

	speed.y += 1;

	position.x += speed.x;
	position.y += speed.y;

	collider->SetPos(position.x, position.y);


	return true;
}

bool j1Green_Fruit::PostUpdate()
{
	Draw();
	if (dead.Finished())
	{
		App->entityManager->DestroyThisEntity(this);
	}
	return true;
}

bool j1Green_Fruit::CleanUp()
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

void j1Green_Fruit::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_fruit);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Green_Fruit::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && current_animation != &dead)
		{
			App->audio->PlayFx(fruit_fx);
			current_animation = &dead;

			//score + x
			//fruits destroyed + 1

		}
	}
}

bool j1Green_Fruit::Load(pugi::xml_node&)
{
	return true;
}
bool j1Green_Fruit::Save(pugi::xml_node&) const
{
	return true;
}
