#include "j1App.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Yellow_Fruit.h"
#include "j1Scene.h"
#include "j1Input.h"

j1Yellow_Fruit::j1Yellow_Fruit(int x, int y, EntityType Type) : j1Entity(x, y, Type)
{
	current_animation = NULL;

	idle.LoadAnimations("red_fruit", "idle");
	dead.LoadAnimations("red_fruit", "dead");
	position.x = x;
	position.y = y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

j1Yellow_Fruit::~j1Yellow_Fruit() {}

bool j1Yellow_Fruit::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/spritesheet.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 65, 65 }, COLLIDER_FRUIT, App->entityManager);
	}

	fruit_fx = App->audio->LoadFx("audio/fx/get_coin.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Yellow_Fruit::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Bat Update", Profiler::Color::Green);

	dt_fruit = dt;
	collider->SetPos(position.x, position.y);


	return true;
}

bool j1Yellow_Fruit::PostUpdate()
{
	Draw();
	if (dead.Finished())
	{
		App->entityManager->DestroyThisEntity(this);
	}
	return true;
}

bool j1Yellow_Fruit::CleanUp()
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

void j1Yellow_Fruit::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_fruit);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Yellow_Fruit::OnCollision(Collider* c1, Collider* c2)
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

bool j1Yellow_Fruit::Load(pugi::xml_node&)
{
	return true;
}
bool j1Yellow_Fruit::Save(pugi::xml_node&) const
{
	return true;
}
