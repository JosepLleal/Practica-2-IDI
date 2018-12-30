#include "j1App.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Red_Fruit.h"
#include "j1Scene.h"
#include "j1Input.h"

j1Red_Fruit::j1Red_Fruit(int x, int y, EntityType Type, int speed_y, int speed_x) : j1Entity(x, y, Type, speed_y, speed_x)
{
	current_animation = NULL;

	idle.LoadAnimations("red_fruit", "idle");
	dead.LoadAnimations("red_fruit", "dead");
	position.x = x;
	position.y = y;

	speed.x = speed_x;
	speed.y = speed_y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

j1Red_Fruit::~j1Red_Fruit() {}

bool j1Red_Fruit::Start()
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
	fall_fx = App->audio->LoadFx("audio/fx/loselife1.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Red_Fruit::Update(float dt, bool do_logic)
{
	dt_fruit = dt;

	if (!App->scene->pause)
	{
		if (speed.y < 15)
		{
			speed.y += 1;
		}
		if (current_animation == &idle)
		{
			position.x += speed.x;
			position.y += speed.y;
		}
	}

	collider->SetPos(position.x , position.y );

	
	return true;
}

bool j1Red_Fruit::PostUpdate()
{
	Draw();
	if (dead.Finished())
	{
		App->entityManager->DestroyThisEntity(this);
	}
	else if (position.y > 1000)
	{
		App->audio->PlayFx(fall_fx);
		App->entityManager->DestroyThisEntity(this);
		App->scene->lifes--;
		if (App->scene->mode1)
			App->scene->fallen_fruits_mode1++;
		else if (App->scene->mode2)
			App->scene->fallen_fruits_mode2++;
	}
	return true;
}

bool j1Red_Fruit::CleanUp()
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

void j1Red_Fruit::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_fruit);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Red_Fruit::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		if (App->scene->mode1)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && current_animation != &dead)
			{
				App->audio->PlayFx(fruit_fx);
				current_animation = &dead;
				App->scene->score += 2;
				App->scene->fruits_cut_mode1++;

			}
		}
		else if (App->scene->mode2)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && current_animation != &dead)
			{
				App->audio->PlayFx(fruit_fx);
				current_animation = &dead;
				App->scene->score += 2;
				App->scene->fruits_cut_mode2++;

			}
		}

	}
}

bool j1Red_Fruit::Load(pugi::xml_node&)
{
	return true;
}
bool j1Red_Fruit::Save(pugi::xml_node&) const
{
	return true;
}
