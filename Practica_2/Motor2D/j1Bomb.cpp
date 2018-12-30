#include "j1App.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Bomb.h"
#include "j1Scene.h"
#include "j1Input.h"

j1Bomb::j1Bomb(int x, int y, EntityType Type, int speed_y, int speed_x) : j1Entity(x, y, Type, speed_y, speed_x)
{
	current_animation = NULL;

	idle.LoadAnimations("bomb", "idle");
	dead.LoadAnimations("bomb", "dead");
	position.x = x;
	position.y = y;

	speed.x = speed_x;
	speed.y = speed_y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

j1Bomb::~j1Bomb() {}

bool j1Bomb::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/spritesheet.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 65, 65 }, COLLIDER_FRUIT, App->entityManager);
	}

	bomb_fx = App->audio->LoadFx("audio/fx/explosion1.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Bomb::Update(float dt, bool do_logic)
{
	dt_bomb = dt;

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

	

	collider->SetPos(position.x, position.y+30);


	return true;
}

bool j1Bomb::PostUpdate()
{
	Draw();
	if (dead.Finished())
	{
		App->entityManager->DestroyThisEntity(this);
	}
	else if (position.y > 1000)
	{
		App->entityManager->DestroyThisEntity(this);
	}
	return true;
}

bool j1Bomb::CleanUp()
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

void j1Bomb::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_bomb);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Bomb::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		if (App->scene->mode1)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && current_animation != &dead)
			{
				App->audio->PlayFx(bomb_fx);
				current_animation = &dead;
				App->scene->lifes--;
				App->scene->score -= 10;
				App->scene->bombs_triggered_mode1++;

			}
		}
		else if (App->scene->mode2)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && current_animation != &dead)
			{
				App->audio->PlayFx(bomb_fx);
				current_animation = &dead;
				App->scene->lifes--;
				App->scene->score -= 10;
				App->scene->bombs_triggered_mode2++;

			}
		}
		
	}
}

bool j1Bomb::Load(pugi::xml_node&)
{
	return true;
}
bool j1Bomb::Save(pugi::xml_node&) const
{
	return true;
}
