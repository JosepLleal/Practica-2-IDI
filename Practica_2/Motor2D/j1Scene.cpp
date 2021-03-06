#include "p2Defs.h"
#include "p2Log.h"
#include "string"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Fadetoblack.h"
#include "j1Player.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "j1Gui_Elements.h"
#include <time.h>


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
		
	level* menu = new level(0, "map_menu.tmx"); 
	level* lvl1 = new level(1, "map_p1.tmx");
	
	levels_list.add(menu); 
	levels_list.add(lvl1);

	level_to_load = levels_list.start;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret; 
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(levels_list.start->data->mapPath.GetString());
	
	actual_level = 0;
	App->render->camera.x = 0; 
	App->render->camera.y = 0; 

	Create_UI_Elements();

	App->audio->musicVolume = Slider_Music->Value_percentage;
	App->audio->sfxVolume = Slider_FX->Value_percentage;
	App->audio->SetMusicVolume();
	App->audio->SetSfxVolume();
	App->audio->PlayMusic("audio/music/main_menu.ogg");

	srand(time(NULL));

	lifes = 3;



	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::Blue);

	Prev_Mouse_Pos = Mouse_Pos;

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Green);

	dt_scene = dt;

	// Gui Logic ------------------------------------------------------------------------------------------

	for (p2List_item<Gui_Elements*>* iterator = App->gui->List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
		{
			if (iterator->data->funct == Function::PLAY1)
			{
				App->fade->FadeToBlack(this, this, 1.3f);
				loading_lvl1 = true;
				if(!mode1)
					mode1 = true;
				if (mode2)
					mode2 = false;
			}
			else if (iterator->data->funct == Function::PLAY2)
			{
				App->fade->FadeToBlack(this, this, 1.3f);
				loading_lvl1 = true;
				

				if (!mode2)
					mode2 = true;
				if (mode1)
					mode1 = false;
			}
			else if (iterator->data->funct == Function::SETTINGS )
			{
				pause = !pause;
				Main_Menu->visible = !Main_Menu->visible;
				Settings_Menu->visible = !Settings_Menu->visible;
			}
			else if (iterator->data->funct == Function::CREDITS)
			{
				pause = !pause;
				Main_Menu->visible = !Main_Menu->visible;
				Credits_Menu->visible = !Credits_Menu->visible;
			}
			else if (iterator->data->funct == Function::EXIT)
			{
				return false;
			}
			

		}
		iterator->data->do_action = false;
		if (iterator->data->type == Element_type::IMAGE)
		{
			if (iterator->data == Menu)
			{

				for (p2List_item<Gui_Elements*>* iterator = Menu->childrens.start; iterator != nullptr; iterator = iterator->next)
				{
					if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
					{
						if (iterator->data->funct == Function::RESUME)
						{
							pause = !pause;
							Menu->visible = !Menu->visible;
						}
						else if (iterator->data->funct == Function::SAVE && actual_level != 3)
						{
							start_pos = false;
							App->SaveGame();
							
						}
						else if (iterator->data->funct == Function::SETTINGS)
						{
							Menu->visible = !Menu->visible;
							Settings_Menu->visible = !Settings_Menu->visible;
							
						}
						else if (iterator->data->funct == Function::MENU)
						{
							App->fade->FadeToBlack(this, this, 1.3f);
							loading_menu = true;
						}
					}
					
				}
			}
			else if (iterator->data == Settings_Menu)
			{
				for (p2List_item<Gui_Elements*>* iterator = Settings_Menu->childrens.start; iterator != nullptr; iterator = iterator->next)
				{
					if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
					{
						if (iterator->data->funct == Function::RESUME)
						{
							if (actual_level == 0)
							{
								pause = !pause;
								Settings_Menu->visible = !Settings_Menu->visible;
								Main_Menu->visible = !Main_Menu->visible;
							}
							else
							{
								Menu->visible = !Menu->visible;
								Settings_Menu->visible = !Settings_Menu->visible;
							}


						}
						else if (iterator->data->funct == Function::APPLY)
						{
							if (actual_level == 0)
							{
								pause = !pause;
								Settings_Menu->visible = !Settings_Menu->visible;
								Main_Menu->visible = !Main_Menu->visible;
							}
							else
							{
								Menu->visible = !Menu->visible;
								Settings_Menu->visible = !Settings_Menu->visible;
							}
							Music_Slider_pos = (Slider_Music->Rect.w * Slider_Music->Value_percentage)/100;
							FX_Slider_pos = (Slider_FX->Rect.w * Slider_FX->Value_percentage) / 100;
							App->audio->musicVolume = Slider_Music->Value_percentage;
							App->audio->sfxVolume = Slider_FX->Value_percentage;
							App->audio->SetMusicVolume();
							App->audio->SetSfxVolume();
						}
					}
					
				}
			
			}
			else if (iterator->data == Credits_Menu)
			{
				for (p2List_item<Gui_Elements*>* iterator = Credits_Menu->childrens.start; iterator != nullptr; iterator = iterator->next)
				{
					if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
					{
						if (iterator->data->funct == Function::RESUME)
						{
							pause = !pause;
							Credits_Menu->visible = !Credits_Menu->visible;
							Main_Menu->visible = !Main_Menu->visible;
						}
						else if (iterator->data->funct == Function::GITHUB_JOAN)
						{
							ShellExecuteA(NULL, "open", "https://github.com/JoanBarduena", NULL, NULL, SW_SHOWNORMAL);
						}
						else if (iterator->data->funct == Function::GITHUB_JOSEP)
						{
							ShellExecuteA(NULL, "open", "https://github.com/JosepLleal", NULL, NULL, SW_SHOWNORMAL);
						}
					}
				}
			}
		}
	}
	// -------------------------------------------------------------------------------------------

	Slider_Music_Value->ValuetoString(Slider_Music->Value_percentage, Slider_Music_Value->text);
	Slider_FX_Value->ValuetoString(Slider_FX->Value_percentage, Slider_FX_Value->text);

	if(Score != nullptr)
		Score->ValuetoString(score, Score->text);

	if (Timer != nullptr)
		//Timer->SetHour(App->entityManager->player->timer); 

	if (Score_num != nullptr)
		//Score_num->ValuetoString(App->entityManager->player->player_score, Score_num->text);

	//F1 Starts form the very first level 
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !pause)
	{
		start_pos = true;
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_lvl1 = true;
	}

	//F4 Go to main menu
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !pause)
	{
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_menu = true;
	}
	//F5 SAVE
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !pause && actual_level != 0 && actual_level != 3)
	{
		start_pos = false;
		App->SaveGame();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN && !pause)
	{
		debug_UI = !debug_UI;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN && !pause)
	{
		App->caped_frames = !App->caped_frames;
	}

	if (lifes <= 0)
	{
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_menu = true;
	}
	
	

	//From fade to levels
	if (App->fade->IsFading() == false)
	{
		if (loading_menu == true)
		{
			HideCursor(false);
			Level_Load(0); 
			if (pause)
				pause = false;
			loading_menu = false; 
		}
		if (loading_lvl1 == true)
		{
			Level_Load(1);
			loading_lvl1 = false; 
		}
		
	}
	

	App->map->Draw();

	
	for (p2List_item<Gui_Elements*>* iterator = App->gui->List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->in_game)
		{
			iterator->data->Draw_InGame_Element();
		}
	}

	App->input->GetMousePosition(Mouse_Pos.x, Mouse_Pos.y);


	LOG("Timer: %i", timer.Read());

	if (actual_level == 1 && pause == false)
	{
		
			if (timer.Read() > 2000 && timer.Read() < 2050)
			{
				RespawnEntities(1);
			}
			else if (timer.Read() > 5000 && timer.Read() < 5050)
			{
				RespawnEntities(2);
			}
			else if (timer.Read() > 9000 && timer.Read() < 9025)
			{
				RespawnEntities(3);
			}
			else if (timer.Read() > 13000 && timer.Read() < 13050)
			{
				RespawnEntities(4);
			}
			else if (timer.Read() > 17000 && timer.Read() < 17050)
			{
				RespawnEntities(5);
			}
			else if (timer.Read() > 21000 && timer.Read() < 21050)
			{
				RespawnEntities(6);
			}
			else if (timer.Read() > 24000 && timer.Read() < 24050)
			{
				RespawnEntities(7);
			}
			else if (timer.Read() > 28000 && timer.Read() < 28050)
			{
				RespawnEntities(8);
			}
			else if (timer.Read() > 31500 && timer.Read() < 31550)
			{
				RespawnEntities(9);
			}
			else if (timer.Read() > 34000 && timer.Read() < 34050)
			{
				RespawnEntities(10);
			}
			else if (timer.Read() > 37000 && timer.Read() < 37050)
			{
				RespawnEntities(11);
			}
			else if (timer.Read() > 40000 && timer.Read() < 40050)
			{
				RespawnEntities(12);
			}
			else if (timer.Read() > 42500 && timer.Read() < 42550)
			{
				RespawnEntities(13);
			}
			else if (timer.Read() > 45500 && timer.Read() < 45550)
			{
				RespawnEntities(14);
			}
			else if (timer.Read() > 48000 && timer.Read() < 48050)
			{
				RespawnEntities(15);
			}
			else if (timer.Read() > 51000 && timer.Read() < 51050)
			{
				RespawnEntities(16);
			}
			else if (timer.Read() > 53500 && timer.Read() < 53550)
			{
				RespawnEntities(17);
			}
			else if (timer.Read() > 56000 && timer.Read() < 56050)
			{
				RespawnEntities(18);
			}
			else if (timer.Read() > 59000 && timer.Read() < 59050)
			{
				RespawnEntities(19);
			}
			else if (timer.Read() > 62000 && timer.Read() < 62050)
			{
				RespawnEntities(20);
			}
			else if (timer.Read() > 68000 && timer.Read() < 68050)
			{
				App->gui->Create_Label(Element_type::LABEL, { 250, 300 }, true, true, " CONGRATULATIONS", { 255,255,255,0 }, App->font->bigger, nullptr);
				App->gui->Create_Label(Element_type::LABEL, { 250, 400 }, true, true, "LEVEL COMPLETED!!", { 255,255,255,0 }, App->font->bigger, nullptr);
			}
			else if (timer.Read() > 70000 && timer.Read() < 70050)
			{
				App->fade->FadeToBlack(this, this, 1.3f);
				loading_menu = true;
			}

	}


	if (lifes != 0 && actual_level == 1)
	{
		if (lifes == 3)
			App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1550, 139, 114, 57 }, true, true, false, App->gui->GetAtlas(), nullptr);
		if (lifes == 2)
			App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1550, 201, 114, 57 }, true, true, false, App->gui->GetAtlas(), nullptr);
		if (lifes == 1)
			App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1550, 260, 114, 57 }, true, true, false, App->gui->GetAtlas(), nullptr);
	}
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::Red);

	bool ret = true;

	if (actual_level != 0 && !App->scene->pause)
	{
		if (loading_saved_game)
			int_timer = timer.Read() + saved_timer;

		else
			int_timer = timer.Read();

		paused_timer = int_timer;
	}
	else
	{
		int_timer = paused_timer;

		timer.Start();
		timer.DefineStartTime(-paused_timer);
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && App->fade->IsFading() == false)
	{
		timer.Stop();
		if (actual_level != 0)
		{
			if (Menu->visible)
			{
				pause = !pause;
				Menu->visible = !Menu->visible;
				HideCursor(true);
			
			}
			else if (!Menu->visible && !Settings_Menu->visible)
			{
				pause = !pause;
				Menu->visible = !Menu->visible;
				HideCursor(false);
				
			}
			else if (!Menu->visible && Settings_Menu->visible)
			{
				Menu->visible = !Menu->visible;
				Settings_Menu->visible = !Settings_Menu->visible;
				
			}
		}
		else
		{

			if (Settings_Menu->visible)
			{
				Settings_Menu->visible = !Settings_Menu->visible;
				Main_Menu->visible = !Main_Menu->visible;
				pause = !pause;
				
			}
			else if (Credits_Menu->visible)
			{
				Credits_Menu->visible = !Credits_Menu->visible;
				Main_Menu->visible = !Main_Menu->visible;
				pause = !pause;
			
			}
		}
	}
	
	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	
	return true;
}

void j1Scene::Level_Load(uint number)
{
	p2List_item<level*>* lvl = levels_list.start;
	for (int i = 0; i < number; i++)
	{
		lvl = lvl->next;
	}
	level_to_load = lvl;


	// From main menu to the level 1 
	if (actual_level == 0 && level_to_load->data->lvl == 1)
	{
		App->audio->PlayMusic("audio/music/ingame.ogg");
		App->gui->Delete_UI_Elements();
		App->map->Load(level_to_load->data->mapPath.GetString());
		App->entityManager->AddPlayer();
		App->entityManager->Start();

		Create_UI_Elements();

		actual_level = level_to_load->data->lvl;
		timer.Start();
	}
	// From level 1 to main menu
	else if (actual_level == 1 && level_to_load->data->lvl == 0)
	{
		if (mode1)
		{
			games_played_mode1++;
		}
		else if (mode2)
		{
			games_played_mode2++;
		}

		if (lifes == 0)
		{
			if(mode1)
				loses_mode1++;
			if(mode2)
				loses_mode2++;
		}
		else
		{
			if (mode1)
				wins_mode1++;
			if (mode2)
				wins_mode2++;
		}

		App->SaveGame();

		App->audio->PlayMusic("audio/music/main_menu.ogg");
		App->entityManager->DestroyAllEntities();
		Score = nullptr; 
		Timer = nullptr; 
		Score_num = nullptr;
		App->gui->Delete_UI_Elements();
		App->map->Load(level_to_load->data->mapPath.GetString());
		Create_UI_Elements();
		actual_level = level_to_load->data->lvl;
		lifes = 3;
		score = 0;

		
				
	}


}

EntityType j1Scene::RandomEntity()
{
	EntityType type;

	int val1 = rand() % (5 + 1);
	
	if (val1 == 0)
		type = RED_FRUIT;
	else if (val1 == 1)
		type = YELLOW_FRUIT;
	else if (val1 == 2)
		type = BLUE_FRUIT;
	else if (val1 == 3)
		type = GREEN_FRUIT;
	else if (val1 == 4)
		type = PURPLE_FRUIT;
	else if (val1 == 5)
		type = BOMB;

	return type;
}


void j1Scene::RespawnEntities(int round)
{
	
	if (round == 1)
	{
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -30, 4);
		App->entityManager->CreateEntity(300, 768, RandomEntity(), -35, 5);
		
	}
	else if (round == 2)
	{
		App->entityManager->CreateEntity(600, 768, RandomEntity(), -25, 2);
		App->entityManager->CreateEntity(300, 768, RandomEntity(), -28, 0);
		App->entityManager->CreateEntity(800, 768, RandomEntity(), -40, -10);
	}
	else if (round == 3)
	{
		App->entityManager->CreateEntity(600, 768, BOMB, -35, 0);
	}
	else if (round == 4)
	{
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -30, 4);
		App->entityManager->CreateEntity(300, 768, RandomEntity(), -35, 5);
		App->entityManager->CreateEntity(600, 768, RandomEntity(), -25, 2);
		App->entityManager->CreateEntity(300, 768, RandomEntity(), -28, 0);
	}
	else if (round == 5)
	{
		App->entityManager->CreateEntity(200, 768, RandomEntity(), -30, 4);
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -35, 5);
		App->entityManager->CreateEntity(600, 768, RandomEntity(), -25, -2);
		App->entityManager->CreateEntity(800, 768, RandomEntity(), -28, -5);
	}
	else if (round == 6)
	{
		App->entityManager->CreateEntity(200, 768, BOMB, -30, 0);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -35, 0);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -25, 0);
		App->entityManager->CreateEntity(800, 768, BOMB, -30, 0);
	}
	else if (round == 7)
	{
		App->entityManager->CreateEntity(200, 768, BOMB, -27, 6);
		App->entityManager->CreateEntity(375, 768, BOMB, -30, 3);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -30, -2);
		App->entityManager->CreateEntity(650, 768, RandomEntity(), -26, -3);
		App->entityManager->CreateEntity(750, 768, RandomEntity(), -29, -4);
	}
	else if (round == 8)
	{
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -31, 2);
		App->entityManager->CreateEntity(300, 768, RandomEntity(), -27, 3);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -33, -6);
		App->entityManager->CreateEntity(950, 768, RandomEntity(), -30, -4);
	}
	else if (round == 9)
	{
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -35, 0);
		App->entityManager->CreateEntity(250, 768, RandomEntity(), -30, 4);
		App->entityManager->CreateEntity(400, 768, BOMB, -28, 3);
		App->entityManager->CreateEntity(700, 768, RandomEntity(), -26, -2);
		App->entityManager->CreateEntity(775, 768, BOMB, -30, 0);
	}
	else if (round == 10)
	{
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -35, 4);
		App->entityManager->CreateEntity(250, 768, RandomEntity(), -28, 0);
		App->entityManager->CreateEntity(350, 768, RandomEntity(), -31, 5);
		App->entityManager->CreateEntity(400, 768, BOMB, -30, 10);
		App->entityManager->CreateEntity(550, 768, RandomEntity(), -27, -2);
		App->entityManager->CreateEntity(600, 768, BOMB, -27, 0);
		
	}
	else if (round == 11)
	{
		App->entityManager->CreateEntity(200, 768, BOMB, -28, 6);
		App->entityManager->CreateEntity(350, 768, BOMB, -30, 4);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -25, 2);
		App->entityManager->CreateEntity(800, 768, BOMB, -32, -9);
	}
	else if (round == 12)
	{
		App->entityManager->CreateEntity(300, 768, BOMB, -32, 10);	
		App->entityManager->CreateEntity(400, 768, RandomEntity(), -27, 10);
		App->entityManager->CreateEntity(400, 768, BOMB, -34, 10);
	}
	else if (round == 13)
	{
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -35, -5);
		App->entityManager->CreateEntity(550, 768, RandomEntity(), -25, -2);
	}
	else if (round == 14)
	{
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -35, 5);
		App->entityManager->CreateEntity(250, 768, BOMB, -30, 8);
		App->entityManager->CreateEntity(450, 768, RandomEntity(), -28, 7);
		App->entityManager->CreateEntity(500, 768, BOMB, -35, 4);
		App->entityManager->CreateEntity(650, 768, RandomEntity(), -32, -5);
		App->entityManager->CreateEntity(850, 768, RandomEntity(), -28, -7);
	}
	else if (round == 15)
	{
		App->entityManager->CreateEntity(100, 768, BOMB, -29, 10);
		App->entityManager->CreateEntity(250, 768, BOMB, -30, 7);
		App->entityManager->CreateEntity(450, 768, RandomEntity(), -28, 8);
		App->entityManager->CreateEntity(500, 768, BOMB, -35, 4);
		App->entityManager->CreateEntity(650, 768, RandomEntity(), -32, -3);
		App->entityManager->CreateEntity(850, 768, BOMB, -28, -7);
		App->entityManager->CreateEntity(900, 768, RandomEntity(), -32, -9);
	}
	else if (round == 16)
	{
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -40, 12);
		App->entityManager->CreateEntity(250, 768, RandomEntity(), -34, 8);
		App->entityManager->CreateEntity(600, 768, RandomEntity(), -40, -12);
		App->entityManager->CreateEntity(800, 768, RandomEntity(), -37, -10);
	}
	else if (round == 17)
	{
		App->entityManager->CreateEntity(50, 768, BOMB, -36, 17);
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -40, 15);
		App->entityManager->CreateEntity(250, 768, RandomEntity(), -34, 10);
		App->entityManager->CreateEntity(575, 768, BOMB, -32, 1);
		App->entityManager->CreateEntity(850, 768, BOMB, -32, -10);
		App->entityManager->CreateEntity(950, 768, RandomEntity(), -37, -16);
	}
	else if (round == 18)
	{
		App->entityManager->CreateEntity(50, 768, BOMB, -36, 17);
		App->entityManager->CreateEntity(100, 768, RED_FRUIT, -25, 9);
		App->entityManager->CreateEntity(300, 768, YELLOW_FRUIT, -27, 0);
		App->entityManager->CreateEntity(350, 768, GREEN_FRUIT, -23, 15);
		App->entityManager->CreateEntity(575, 768, BOMB, -40, -3);
		App->entityManager->CreateEntity(650, 768, BLUE_FRUIT, -29, -6);
		App->entityManager->CreateEntity(850, 768, BOMB, -32, -14);
	}
	else if (round == 19)
	{
		App->entityManager->CreateEntity(50, 768, RandomEntity(), -33, 0);
		App->entityManager->CreateEntity(100, 768, BOMB, -28, 0);
		App->entityManager->CreateEntity(350, 768, BOMB, -23, 0);
		App->entityManager->CreateEntity(400, 768, RandomEntity(), -25, 0);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -27, 0);
		App->entityManager->CreateEntity(650, 768, RandomEntity(), -29, 0);
		App->entityManager->CreateEntity(850, 768, RandomEntity(), -32, 0);
	}
	else if (round == 20)
	{
		App->entityManager->CreateEntity(50, 768, RandomEntity(), -36, 17);
		App->entityManager->CreateEntity(100, 768, RandomEntity(), -25, 9);
		App->entityManager->CreateEntity(300, 768, RandomEntity(), -27, 0);
		App->entityManager->CreateEntity(400, 768, RandomEntity(), -25, 4);
		App->entityManager->CreateEntity(500, 768, RandomEntity(), -27, 9);
		App->entityManager->CreateEntity(850, 768, RandomEntity(), -32, -14);
	}


	App->entityManager->Start();

}

void j1Scene::Create_UI_Elements()
{
	if (level_to_load->data->lvl == 0)
	{

		Main_Menu = App->gui->Create_Image(Element_type::IMAGE, { 0, 0 }, {4000, 4000, (int)App->win->width, (int)App->win->height}, true, false, false, App->gui->GetAtlas(), nullptr);

		/*SDL_Rect title_rect = { 0, 0, 500, 300 };
		App->gui->Create_Image(Element_type::IMAGE, { 275, -20 }, title_rect, true, false, false, title, Main_Menu);*/

		Gui_Elements* Play = App->gui->Create_Button(Element_type::BUTTON, { 415, 300 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::PLAY1, Main_Menu);
		Gui_Elements* Play2 = App->gui->Create_Button(Element_type::BUTTON, { 415, 370 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::PLAY2, Main_Menu);
		Gui_Elements* MainMenu_Settings = App->gui->Create_Button(Element_type::BUTTON, { 415, 440 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::SETTINGS, Main_Menu);
		Gui_Elements* Credits = App->gui->Create_Button(Element_type::BUTTON, { 415, 510 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::CREDITS, Main_Menu);
		Gui_Elements* Exit = App->gui->Create_Button(Element_type::BUTTON, { 415, 580 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::EXIT, Main_Menu);

	
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, true, true, "DRAG MODE", { 255,255,255,0 }, App->font->default, Play);
		App->gui->Create_Label(Element_type::LABEL, { 20, 6 }, true, true, "CLICK MODE", { 255,255,255,0 }, App->font->default, Play2);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, true, true, "SETTINGS", { 255,255,255,0 }, App->font->default, MainMenu_Settings);
		App->gui->Create_Label(Element_type::LABEL, { 40, 6 }, true, true, "CREDITS", { 255,255,255,0 }, App->font->default, Credits);
		App->gui->Create_Label(Element_type::LABEL, { 60, 6 }, true, true, "EXIT", { 255,255,255,0 }, App->font->default, Exit);

		Credits_Menu = App->gui->Create_Image(Element_type::IMAGE, { 120, 80 }, { 621, 377, 785, 568 }, false, false, false, App->gui->GetAtlas());

		Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 765, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Credits_Menu);
		Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);

		App->gui->Create_Label(Element_type::LABEL, { 37, 40 }, true, true, "MIT License", { 100,40, 0 ,0 }, App->font->default, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 90 }, true, true, "Copyright (c) 2018 ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 110 }, true, true, "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 130 }, true, true, "files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy,", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 150 }, true, true, "modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 170 }, true, true, "Software is furnished to do so, subject to the following conditions:", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 200 }, true, true, "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 230 }, true, true, "THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 250 }, true, true, "LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 270 }, true, true, "NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 290 }, true, true, "WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 310 }, true, true, "SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);


	}
	else if (level_to_load->data->lvl == 1)
	{
		
		if (level_to_load->data->lvl == 1 )
		{
			HideCursor(true);
			
			Gui_Elements* Coin = App->gui->Create_Image(Element_type::IMAGE, {800, 18}, {648, 33, 75, 70}, true, true, false, App->gui->GetAtlas(), nullptr); 
			Score = App->gui->Create_Label(Element_type::LABEL, { 100, -10 }, true, true, score, { 255, 255, 255, 0 }, App->font->bigger, Coin);
			
			/*Gui_Elements* Score = App->gui->Create_Label(Element_type::LABEL, { 820, 720 }, true, true, "SCORE:", { 255, 255, 255, 0 }, App->font->default, nullptr);
			Score_num = App->gui->Create_Label(Element_type::LABEL, {100, 0 }, true, true, "0", { 255, 255, 255, 0 }, App->font->default, Score);*/
		}

		Menu = App->gui->Create_Image(Element_type::IMAGE, { 355, 200 }, { 8, 459, 315, 402 }, false, false, false, App->gui->GetAtlas());
		
		Gui_Elements* Resume = App->gui->Create_Button(Element_type::BUTTON, { 62, 115 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::RESUME, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 35, 6 }, false, false, "RESUME", { 255,255,255,0 }, App->font->default, Resume);

		Gui_Elements* Main_Menu = App->gui->Create_Button(Element_type::BUTTON, { 62, 255 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::MENU, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, false, false, "MAIN MENU", { 255,255,255,0 }, App->font->default, Main_Menu);

		Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 290, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Menu);
		Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);


	}

	Settings_Menu = App->gui->Create_Image(Element_type::IMAGE, { 205, 160 }, { 6, 7, 611, 442 }, false, false, false, App->gui->GetAtlas());

	Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 586, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Settings_Menu);
	Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);

	Slider_Music = App->gui->Create_Slider(Element_type::SLIDER, { 350, 140 }, { 860,337,190,2 }, false, App->gui->GetAtlas(), Settings_Menu);
	Button_Music = App->gui->Create_Image(Element_type::IMAGE, { Music_Slider_pos, -20 }, { 770, 197, 28, 42 }, false, false, true, App->gui->GetAtlas(), Slider_Music);
	Gui_Elements* Music_Volume = App->gui->Create_Label(Element_type::LABEL, { 100, 120 }, false, false, "Music Volume", { 100,40, 0,0 }, App->font->default, Settings_Menu);
	Slider_Music_Value = App->gui->Create_Label(Element_type::LABEL, { 490, 80 }, false, false, "0%%", { 100,40, 0,0 }, App->font->smaller, Settings_Menu);


	Slider_FX = App->gui->Create_Slider(Element_type::SLIDER, { 350, 230 }, { 860,337,190,2 }, false, App->gui->GetAtlas(), Settings_Menu);
	Button_FX = App->gui->Create_Image(Element_type::IMAGE, { FX_Slider_pos, -20 }, { 770, 197, 28, 42 }, false, false, true, App->gui->GetAtlas(), Slider_FX);
	Gui_Elements* FX_Volume = App->gui->Create_Label(Element_type::LABEL, { 100, 210 }, false, false, "FX Volume", { 100,40, 0,0 }, App->font->default, Settings_Menu);
	Slider_FX_Value = App->gui->Create_Label(Element_type::LABEL, { 490, 170 }, false, false, "0%%", { 100,40, 0,0 }, App->font->smaller, Settings_Menu);

	Gui_Elements* Apply = App->gui->Create_Button(Element_type::BUTTON, { 200, 380 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::APPLY, Settings_Menu);
	App->gui->Create_Label(Element_type::LABEL, { 50, 6 }, false, false, "APPLY", { 255,255,255,0 }, App->font->default, Apply);

	
}

void j1Scene::HideCursor(bool hide)
{
	if (hide)
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
}
