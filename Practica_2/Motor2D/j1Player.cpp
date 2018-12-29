#include "j1App.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Input.h"

j1Player::j1Player(int x, int y, EntityType Type) : j1Entity(x, y, Type, 0, 0)
{
	current_animation = NULL;

	position.x = x;
	position.y = y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

j1Player::~j1Player() {}

bool j1Player::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("gui/atlas.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 28, 26 }, COLLIDER_PLAYER, App->entityManager);
	}

	player_fx = App->audio->LoadFx("audio/fx/squashing.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	rect = { 656,327,35,37 };
	
	added = true;

	return true;
}

bool j1Player::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Bat Update", Profiler::Color::Green);

	dt_player = dt;

	position.x = App->scene->Mouse_Pos.x;
	position.y = App->scene->Mouse_Pos.y;

	if (collider != nullptr)
	{
		collider->SetPos(position.x, position.y);
	}
	

	return true;
}

bool j1Player::PostUpdate()
{
	Draw();
	return true;
}

bool j1Player::CleanUp()
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

void j1Player::Draw()
{
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	
}

bool j1Player::Load(pugi::xml_node&)
{
	return true;
}
bool j1Player::Save(pugi::xml_node&) const
{
	return true;
}
