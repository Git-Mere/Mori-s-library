#include "Book3.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/CameraCom.h"
#include "../Engine/ShowCollision.h"
#include "States.h"
#include "Player.h"
#include "../Engine/Particle.h"
#include "../Engine/Particels.h"
#include "captain_attack.h"
#include "captain.h"
#include "FontObject.h"
#include "Life.h"
#include "../ScreenEffect.h"
#include "Background.h"
#include "GameOver.h"

extern bool book3_clear[2];

extern float SOUND;
void Book3::Load()
{
	background_sound = SOUND - 15.0f;
	if (background_sound < 0) {
		background_sound = 0;
	}
	if (!SoundFlag) {
		SoundEffect::PlaySoundEffect(SFX::Book3BGM);
		SoundEffect::Loop_SoundEffect(SFX::Book3BGM);
		SoundEffect::SetSoundEffectVolume(SFX::Book3BGM, background_sound);
		SoundFlag = true;
	}
	AddGSComponent(new GameObjectManager);
	glCheck(glClearColor(0.0f, 0.68f, 0.94f, 1.0f));
	AddGSComponent(new CameraComponent({ 0,0 }, { 0,0 }));
	AddGSComponent(new Background);
	AddGSComponent(new ShowCollision);
	AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));
	GetGSComponent<Background>()->Add("assets/sprite/Background/Deck.spt", 1, Engine::Instance()->Get_Window());
	GetGSComponent<Background>()->Add("assets/sprite/Background/Ocean.spt", 1, Engine::Instance()->Get_Window());
	if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
		capatin_say = loadQuizSentencesFromFile<Sentence>("assets/textfile/captain_say_easy.txt");
	}
	else {
		capatin_say = loadQuizSentencesFromFile<Sentence>("assets/textfile/captain_say.txt");
	}

	player_ptr = new Player({ -30, -200 }, vec2(50));
	captain = new Captain(vec2(100, 200));
	player_ptr->change_player_scale(vec2(250));
	attack = new Captain_attack(vec2(-90, 10), *player_ptr, *captain);
	say_object = new FontObject(vec2(380, 200), capatin_say.getSentences(attack->GetIndex()));
	say_object->SetFontSize(vec2(300, 200));
	gameclear = new GameClear(vec2(2000, 2000));
	gameover = new GameOver(vec2(2000, 2000));
	pressanykey = new PressAnyKey(vec2(2000, 2000));
	darkscreen = new Screen_Effect(vec2(5000, 5000));

	previous_index = attack->GetIndex();

	AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));

	GetGSComponent<GameObjectManager>()->Add(player_ptr);
	GetGSComponent<GameObjectManager>()->Add(captain);
	GetGSComponent<GameObjectManager>()->Add(say_object);
	GetGSComponent<GameObjectManager>()->Add(attack);

	AddGSComponent(new ParticleManager<Particles::Smoke>());
	dead_cooltime = 0;
	Sound_One_Time = false;
	GetGSComponent<GameObjectManager>()->Add(darkscreen);
	GetGSComponent<GameObjectManager>()->Add(gameover);
	GetGSComponent<GameObjectManager>()->Add(gameclear);
	GetGSComponent<GameObjectManager>()->Add(pressanykey);
}

void Book3::Update(float dt)
{
	UpdateGSComponents(dt);
	GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
	GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());
	Change_say();

	if (player_ptr->GetGOComponent<Life>()->Dead() == true) {
		if (Sound_One_Time == false) {
			// Stop mugic
			SoundEffect::EraseSoundEffect();
			// Add mugic
			SoundEffect::PlaySoundEffect(SFX::Death);
			SoundEffect::Loop_SoundEffect(SFX::Death);
			SoundEffect::SetSoundEffectVolume(SFX::Death, background_sound);
			Sound_One_Time = true;
		}
		gameover->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
		pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
		darkscreen->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y));
		GetGSComponent<GameObjectManager>()->Remove(player_ptr);

		if (dead_cooltime < dead_cooltime_max) {
			dead_cooltime += dt;
		}
		else {
			if (Engine::GetInput().PressAnyKey()) {
				Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Libarary));
			}
		}
	}

	if (attack->GetFinish()) {
		if (Sound_One_Time == false) {
			SoundEffect::Stop_Loop_SoundEffect(SFX::Book3BGM);
			SoundEffect::EraseSoundEffect();
			SoundEffect::PlaySoundEffect(SFX::ClearBGM);
			SoundEffect::SetSoundEffectVolume(SFX::ClearBGM, background_sound);

			Sound_One_Time = true;
		}

		gameclear->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
		pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
		darkscreen->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y));
		player_ptr->SetVelocity(vec2(0, 0));
		player_ptr->Set_invin_timer(5.0f);

		if (dead_cooltime < dead_cooltime_max) {
			dead_cooltime += dt;
		}
		else {
			if (Engine::GetInput().PressAnyKey()) {
				if (Engine::Instance()->Get_Game_mode() == Engine::Easy) {
					book3_clear[0] = true;
				}
				else if (Engine::Instance()->Get_Game_mode() == Engine::Hard) {
					book3_clear[1] = true;
				}
				Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Libarary));
			}
		}
	}

	if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
		SoundEffect::EraseSoundEffect();
		Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
	}
}

void Book3::Draw()
{
	glCheck(glClear(GL_COLOR_BUFFER_BIT));
	GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());
	GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
	GetGSComponent<UIManager>()->AllDraw();
}

void Book3::Change_say()
{
	if (previous_index != attack->GetIndex()) {
		say_object->change_text(capatin_say.getSentences(attack->GetIndex()));
		say_object->SetFontSize(attack->Get_font_size());
		previous_index = attack->GetIndex();
	}
}

void Book3::Unload()
{
	if (SoundFlag) {
		SoundEffect::Stop_Loop_SoundEffect(SFX::Book3BGM);
		SoundEffect::Stop_Loop_SoundEffect(SFX::ClearBGM);
		SoundEffect::StopSoundEffect(SFX::ClearBGM);
		SoundFlag = false;
	}
	GetGSComponent<GameObjectManager>()->Unload();
	ClearGSComponents();
	SoundEffect::EraseSoundEffect();
}