#include "../Engine/GameObjectManager.h"
#include "../Engine/ShowCollision.h"
#include "../Engine/CameraCom.h"
#include "../Engine/Particels.h"

#include "Book2.h"
#include "Player.h"
#include "QuizBox.h"
#include "States.h"
#include "Timer.h"
#include "sample_logo.h"
#include "../Engine/Particels.h"
#include "MagicBall.h"
#include "FontObject.h"
#include "Block.h"
#include "Background.h"
#include "SkillUI.h"
#include "MathPerson.h"
#include "Life.h"
#include "Sphinx.h"
#include "../ScreenEffect.h"
#include "../Wall.h"
#include "GameOver.h"
extern float SOUND;
extern bool book2_clear;

void Book2::Load()
{
    background_sound = SOUND - 15.0f;
    if (background_sound < 0) {
        background_sound = 0.0f;
    }

    if (!SoundFlag) {
        SoundEffect::PlaySoundEffect(SFX::Book2BGM);
        SoundEffect::Loop_SoundEffect(SFX::Book2BGM);
        SoundEffect::SetSoundEffectVolume(SFX::Book2BGM, background_sound);
        SoundFlag = true;
    }
    glCheck(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

    AddGSComponent(new GameObjectManager);
    AddGSComponent(new CameraComponent({ 0, -Engine::Instance()->Get_Window().y / 8.0f }, { 0, Engine::Instance()->Get_iWindow().y / 3 }));
    AddGSComponent(new ShowCollision);
    AddGSComponent(new Background);
    AddGSComponent(new UpTimer(0.0f));
    AddGSComponent(new DownTimer(10.0f));
    AddGSComponent(new effect::Shake(*GetGSComponent<CameraComponent>(), 0));
    player_ptr = new Player({ 0, -200 }, vec2(50));
    AddGSComponent(new UIManager(GetGSComponent<CameraComponent>()->Getworld_to_ndc_don_move(), *player_ptr));
    person_ptr = new MathPerson(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f),
        vec2(Engine::Instance()->Get_Window().x / 8.0f, Engine::Instance()->Get_Window().y / 8.0f));
    sphinx_ptr = new Sphinx(sphinx_position, vec2(150), *player_ptr);
    gameover = new GameOver(vec2(2000, 2000));
    gameclear = new GameClear(vec2(2000, 2000));
    pressanykey = new PressAnyKey(vec2(2000, 2000));
    darkscreen = new Screen_Effect(vec2(5000, 5000));

    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(0, 500), vec2(Engine::Instance()->Get_Window().x, 100), "None"));
    GetGSComponent<GameObjectManager>()->Add(new Wall(vec2(0, -500), vec2(Engine::Instance()->Get_Window().x, 100), "None"));

    //Retry setting
    random_quiz_number1 = util::random(1, 6);
    quiz1 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz1-" + std::to_string(random_quiz_number1) + ".txt");
    pattern1_answer = check_answer(quiz1.getSentences(5));

    random_quiz_number2 = util::random(1, 6);
    quiz2 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz2-" + std::to_string(random_quiz_number2) + ".txt");
    pattern2_answer = check_answer(quiz2.getSentences(5));

    random_quiz_number3 = util::random(1, 6);
    quiz3 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz3-" + std::to_string(random_quiz_number3) + ".txt");
    pattern3_answer = check_answer(quiz3.getSentences(5));

    random_quiz_number4 = util::random(1, 6);
    quiz4 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz4-" + std::to_string(random_quiz_number4) + ".txt");
    pattern4_answer = check_answer(quiz4.getSentences(5));

    //quiz and answer
    quiz_sentence = new FontObject(vec2(0, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f),
        vec2(Engine::Instance()->Get_Window().x * 0.8f, Engine::Instance()->Get_Window().y / 5.0f));

    quiz_answer1 = new FontObject(vec2(-Engine::Instance()->Get_Window().x / 2.5f, -Engine::Instance()->Get_Window().y / 2.0f - Engine::Instance()->Get_Window().y / 15.0f),
        Engine::Instance()->Get_Window() / 8.0f);

    quiz_answer2 = new FontObject(vec2(-Engine::Instance()->Get_Window().x / 7.0f, -Engine::Instance()->Get_Window().y / 2.0f - Engine::Instance()->Get_Window().y / 15.0f),
        Engine::Instance()->Get_Window() / 8.0f);

    quiz_answer3 = new FontObject(vec2(Engine::Instance()->Get_Window().x / 7.0f, -Engine::Instance()->Get_Window().y / 2.0f - Engine::Instance()->Get_Window().y / 15.0f),
        Engine::Instance()->Get_Window() / 8.0f);

    quiz_answer4 = new FontObject(vec2(Engine::Instance()->Get_Window().x / 2.5f, -Engine::Instance()->Get_Window().y / 2.0f - Engine::Instance()->Get_Window().y / 15.0f),
        Engine::Instance()->Get_Window() / 8.0f);
    //timer
    quiz_cooltime = new FontObject(vec2(0, 0), Engine::Instance()->Get_Window() / 6.0f);

    //quizbox
    quizBox1 = new QuizBox(block_position[0], &answer1, 1);
    quizBox2 = new QuizBox(block_position[1], &answer2, 2);
    quizBox3 = new QuizBox(block_position[2], &answer3, 3);
    quizBox4 = new QuizBox(block_position[3], &answer4, 4);

    GetGSComponent<GameObjectManager>()->Add(quizBox1);
    GetGSComponent<GameObjectManager>()->Add(quizBox2);
    GetGSComponent<GameObjectManager>()->Add(quizBox3);
    GetGSComponent<GameObjectManager>()->Add(quizBox4);

    //Font
    GetGSComponent<GameObjectManager>()->Add(quiz_sentence);
    GetGSComponent<GameObjectManager>()->Add(quiz_answer1);
    GetGSComponent<GameObjectManager>()->Add(quiz_answer2);
    GetGSComponent<GameObjectManager>()->Add(quiz_answer3);
    GetGSComponent<GameObjectManager>()->Add(quiz_answer4);

    GetGSComponent<GameObjectManager>()->Add(quiz_cooltime);

    //MathPerson
    GetGSComponent<GameObjectManager>()->Add(person_ptr);

    //block
    block_ptr = new Block(vec2(Engine::Instance()->Get_Window() * (-2)), vec2(Engine::Instance()->Get_Window().x / 6 + 10, Engine::Instance()->Get_Window().y / 6 + 10));
    GetGSComponent<GameObjectManager>()->Add(block_ptr);

    //magicball
    AddGSComponent(new ParticleManager<Particles::MagicBall>());
    AddGSComponent(new ParticleManager<Particles::Earthquake>());

    GetGSComponent<Background>()->Add("assets/sprite/Background/book2background.spt", 1, Engine::Instance()->Get_Window());

    //sphinx
    GetGSComponent<GameObjectManager>()->Add(sphinx_ptr);

    //player
    GetGSComponent<GameObjectManager>()->Add(player_ptr);
    AddGSComponent(new ParticleManager<Particles::Smoke>());
    //Retry setting
    magicball_count = 0;
    pattern = 0;//
    stop_move_one_time = false;

    Sound_One_Time = false;
    dead_cooltime = 0;
    GetGSComponent<GameObjectManager>()->Add(darkscreen);
    GetGSComponent<GameObjectManager>()->Add(gameclear);
    GetGSComponent<GameObjectManager>()->Add(gameover);
    GetGSComponent<GameObjectManager>()->Add(pressanykey);
}

void Book2::Update([[maybe_unused]] float dt)
{
    GetGSComponent<CameraComponent>()->Update(player_ptr->GetPosition());

    UpdateGSComponents(dt);
    GetGSComponent<GameObjectManager>()->UpdateAll(dt, GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->Update(dt);
    //Engine::GetLogger().LogDebug(std::to_string(pattern));
    Engine::GetLogger().LogDebug(std::to_string(GetGSComponent <UpTimer>()->Remaining()));

    if (player_ptr->GetGOComponent<Life>()->Dead() == true) {
        pattern = -1;
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
        darkscreen->RePosition( vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y) );
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

    if (Engine::GetInput().KeyJustReleased(Input::Keys::ESC)) {
        Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Mainmenu));
    }

    //answer store
    if (answer1 == true) {
        now_answer = 1;
    }
    else if (answer2 == true) {
        now_answer = 2;
    }
    else if (answer3 == true) {
        now_answer = 3;
    }
    else if (answer4 == true) {
        now_answer = 4;
    }
    else {
        now_answer = 0;
    }

    switch (pattern) {
    case 0:  /////////////////////////////////////////// Ready to Start ///////////////////////////////////////////
        if (sphinx_ptr->SphinxHelth() == sphinx_maxlife - (pattern_life * 1)) {
            person_ptr->SetPersonPosition(vec2(-2000, -2000));
            draw_one_time = true;
            GetGSComponent <UpTimer>()->Set(0.0f);
            GetGSComponent <DownTimer>()->Reset();
            small_pattern = true;
            sphinx_ptr->Rest();
            pattern++;
            SoundEffect::PlaySoundEffect(SFX::Quiz_pop_out);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_pop_out, SOUND);
            SoundEffect::PlaySoundEffect(SFX::Quiz_timer);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_timer, SOUND);
        }
        break;

    case 1:  /////////////////////////////////////////// Quiz ///////////////////////////////////////////
        draw_one_time = false;
        if (GetGSComponent <DownTimer>()->RemainingInt() <= 0) {
            if (now_answer == pattern1_answer && small_pattern == true) {
                SoundEffect::PlaySoundEffect(SFX::Good_answer);
                SoundEffect::SetSoundEffectVolume(SFX::Good_answer, SOUND);
                draw_one_time = true;
                person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                sphinx_ptr->Active(random_position[0]);
                pattern++;
            }
            else {
                if (max_magicball > magicball_count) {
                    if (magicball_count == 1) {
                        SoundEffect::PlaySoundEffect(SFX::Bad_answer);
                        SoundEffect::SetSoundEffectVolume(SFX::Bad_answer, SOUND);
                        SoundEffect::PlaySoundEffect(SFX::Earthquake);
                        SoundEffect::SetSoundEffectVolume(SFX::Earthquake, SOUND);
                    }
                    small_pattern = false;
                    util::using_random_seed();
                    float position_x1 = util::random(-Engine::Instance()->Get_Window().x / 2, Engine::Instance()->Get_Window().x / 2);
                    float position_y1 = util::random(-Engine::Instance()->Get_Window().y / 2, Engine::Instance()->Get_Window().y / 2);
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::MagicBall>>()->Emit(1, vec2(position_x1, position_y1), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                    Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(5.f, 10, 8.0f, 1);
                    magicball_count += 1;
                }
                else {
                    draw_one_time = true;
                    person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                    sphinx_ptr->Active(random_position[0]);
                    pattern++;
                }
            }
        }
        break;

    case 2://///////////////////////////////////////// Ready to Quiz2 ///////////////////////////////////////////
        if (sphinx_ptr->SphinxHelth() == sphinx_maxlife - (pattern_life * 2)) {
            person_ptr->SetPersonPosition(vec2(-2000, -2000));
            draw_one_time = true;
            GetGSComponent <UpTimer>()->Set(0.0f);
            GetGSComponent <DownTimer>()->Set(10.0f);

            quizBox1->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 4, Engine::Instance()->Get_Window().y / 4));
            quizBox2->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 4, Engine::Instance()->Get_Window().y / 4));
            quizBox3->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 4, Engine::Instance()->Get_Window().y / 4));
            quizBox4->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 4, Engine::Instance()->Get_Window().y / 4));

            magicball_count = 0;
            small_pattern = true;
            sphinx_ptr->Rest();
            pattern++;
            SoundEffect::PlaySoundEffect(SFX::Quiz_pop_out);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_pop_out, SOUND);
            SoundEffect::PlaySoundEffect(SFX::Quiz_timer);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_timer, SOUND);
        }
        break;

    case 3://///////////////////////////////////////// Quiz2 ///////////////////////////////////////////
        draw_one_time = false;
        if (GetGSComponent <DownTimer>()->RemainingInt() <= 0) {
            if (now_answer == pattern2_answer && small_pattern == true) {
                SoundEffect::PlaySoundEffect(SFX::Good_answer);
                SoundEffect::SetSoundEffectVolume(SFX::Good_answer, SOUND);
                draw_one_time = true;
                person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                sphinx_ptr->Active(random_position[1]);
                pattern++;
            }
            else {
                if (max_magicball > magicball_count) {
                    if (magicball_count == 1) {
                        SoundEffect::PlaySoundEffect(SFX::Bad_answer);
                        SoundEffect::SetSoundEffectVolume(SFX::Bad_answer, SOUND);
                        SoundEffect::PlaySoundEffect(SFX::Earthquake);
                        SoundEffect::SetSoundEffectVolume(SFX::Earthquake, SOUND);
                    }
                    small_pattern = false;
                    util::using_random_seed();
                    float position_x1 = util::random(-Engine::Instance()->Get_Window().x / 2, Engine::Instance()->Get_Window().x / 2);
                    float position_y1 = util::random(-Engine::Instance()->Get_Window().y / 2, Engine::Instance()->Get_Window().y / 2);
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::MagicBall>>()->Emit(1, vec2(position_x1, position_y1), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                    Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(5.f, 10, 10.f, 1);

                    magicball_count += 1;
                }
                else {
                    draw_one_time = true;
                    sphinx_ptr->Active(random_position[1]);
                    pattern++;
                }
            }
        }
        break;

    case 4://///////////////////////////////////////// Ready to Quiz3 ///////////////////////////////////////////
        if (sphinx_ptr->SphinxHelth() == sphinx_maxlife - (pattern_life * 3)) {
            person_ptr->SetPersonPosition(vec2(-2000, -2000));
            draw_one_time = true;
            GetGSComponent <UpTimer>()->Set(0.0f);
            GetGSComponent <DownTimer>()->Set(10.0f);

            quizBox1->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 6, Engine::Instance()->Get_Window().y / 6));
            quizBox2->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 6, Engine::Instance()->Get_Window().y / 6));
            quizBox3->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 6, Engine::Instance()->Get_Window().y / 6));
            quizBox4->SetBoxScale(vec2(Engine::Instance()->Get_Window().x / 6, Engine::Instance()->Get_Window().y / 6));

            magicball_count = 0;
            small_pattern = true;
            sphinx_ptr->Rest();
            pattern++;
            SoundEffect::PlaySoundEffect(SFX::Quiz_pop_out);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_pop_out, SOUND);
            SoundEffect::PlaySoundEffect(SFX::Quiz_timer);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_timer, SOUND);
        }
        break;

    case 5://///////////////////////////////////////// Quiz3 ///////////////////////////////////////////
        draw_one_time = false;
        if (GetGSComponent <DownTimer>()->RemainingInt() <= 0) {
            if (now_answer == pattern3_answer && small_pattern == true) {
                SoundEffect::PlaySoundEffect(SFX::Good_answer);
                SoundEffect::SetSoundEffectVolume(SFX::Good_answer, SOUND);
                draw_one_time = true;
                person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                person_ptr->SetAnimation(false);
                sphinx_ptr->Active(random_position[2]);
                pattern++;
            }
            else {
                if (max_magicball > magicball_count) {
                    if (magicball_count == 1) {
                        SoundEffect::PlaySoundEffect(SFX::Bad_answer);
                        SoundEffect::SetSoundEffectVolume(SFX::Bad_answer, SOUND);
                        SoundEffect::PlaySoundEffect(SFX::Earthquake);
                        SoundEffect::SetSoundEffectVolume(SFX::Earthquake, SOUND);
                    }
                    small_pattern = false;
                    util::using_random_seed();
                    float position_x1 = util::random(-Engine::Instance()->Get_Window().x / 2, Engine::Instance()->Get_Window().x / 2);
                    float position_y1 = util::random(-Engine::Instance()->Get_Window().y / 2, Engine::Instance()->Get_Window().y / 2);
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::MagicBall>>()->Emit(1, vec2(position_x1, position_y1), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                    Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(5.f, 10, 10.f, 1);

                    magicball_count += 1;
                }
                else {
                    draw_one_time = true;
                    person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                    person_ptr->SetAnimation(false);
                    sphinx_ptr->Active(random_position[2]);
                    pattern++;
                }
            }
        }
        break;

    case 6://///////////////////////////////////////// Ready to Quiz4 ///////////////////////////////////////////
        if (sphinx_ptr->SphinxHelth() == sphinx_maxlife - (pattern_life * 4)) {
            person_ptr->SetPersonPosition(vec2(-2000, -2000));
            draw_one_time = true;
            GetGSComponent <UpTimer>()->Set(0.0f);
            GetGSComponent <DownTimer>()->Set(10.0f);

            person_ptr->SetAnimation(true);
            block_ptr->SetBlockPosition(block_position[pattern4_answer - 1]);
            magicball_count = 0;
            small_pattern = true;
            sphinx_ptr->Rest();
            pattern++;
            SoundEffect::PlaySoundEffect(SFX::Quiz_pop_out);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_pop_out, SOUND);
            SoundEffect::PlaySoundEffect(SFX::Quiz_timer);
            SoundEffect::SetSoundEffectVolume(SFX::Quiz_timer, SOUND);
        }
        break;

    case 7://///////////////////////////////////////// Quiz4 ///////////////////////////////////////////
        draw_one_time = false;
        if (GetGSComponent <DownTimer>()->RemainingInt() <= 0) {
            if (now_answer == pattern4_answer && small_pattern == true) {
                SoundEffect::PlaySoundEffect(SFX::Good_answer);
                SoundEffect::SetSoundEffectVolume(SFX::Good_answer, SOUND);
                draw_one_time = true;
                person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                sphinx_ptr->Active(random_position[3]);
                pattern++;
            }
            else {
                if (max_magicball > magicball_count) {
                    if (magicball_count == 1) {
                        SoundEffect::PlaySoundEffect(SFX::Bad_answer);
                        SoundEffect::SetSoundEffectVolume(SFX::Bad_answer, SOUND);
                        SoundEffect::PlaySoundEffect(SFX::Earthquake);
                        SoundEffect::SetSoundEffectVolume(SFX::Earthquake, SOUND);
                    }
                    small_pattern = false;
                    util::using_random_seed();
                    float position_x1 = util::random(-Engine::Instance()->Get_Window().x / 2, Engine::Instance()->Get_Window().x / 2);
                    float position_y1 = util::random(-Engine::Instance()->Get_Window().y / 2, Engine::Instance()->Get_Window().y / 2);
                    Engine::GetGameStateManager().GetGSComponent<ParticleManager<Particles::MagicBall>>()->Emit(1, vec2(position_x1, position_y1), vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f);
                    Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(5.f, 10, 10.f, 1);

                    magicball_count += 1;
                }
                else {
                    draw_one_time = true;
                    person_ptr->SetPersonPosition(vec2(-Engine::Instance()->Get_Window().x / 2.0f + Engine::Instance()->Get_Window().y / 10.0f, Engine::Instance()->Get_Window().y / 2.0f + Engine::Instance()->Get_Window().y / 15.0f));
                    sphinx_ptr->Active(random_position[3]);
                    pattern++;
                }
            }
        }
        break;

    case 8://///////////////////////////////////////// END ///////////////////////////////////////////
        if (sphinx_ptr->SphinxHelth() == sphinx_maxlife - (pattern_life * 5)) {
            draw_one_time = true;
            GetGSComponent <UpTimer>()->Set(0.0f);
            GetGSComponent <DownTimer>()->Set(10.0f);

            magicball_count = 0;
            small_pattern = true;

            if (Sound_One_Time == false) {
                SoundEffect::Stop_Loop_SoundEffect(SFX::Book2BGM);
                SoundEffect::EraseSoundEffect();
                SoundEffect::PlaySoundEffect(SFX::ClearBGM);
                SoundEffect::SetSoundEffectVolume(SFX::ClearBGM, background_sound);

                Sound_One_Time = true;
            }
            gameclear->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y + 150));
            pressanykey->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y - 150));
            darkscreen->RePosition(vec2(GetGSComponent<CameraComponent>()->GetPosition().x, GetGSComponent<CameraComponent>()->GetPosition().y));
            player_ptr->Set_stop_move(true);
            player_ptr->SetVelocity(vec2(0, 0));
            player_ptr->Set_invin_timer(5.0f);

            if (dead_cooltime < dead_cooltime_max) {
                dead_cooltime += dt;
            }
            else {
                if (stop_move_one_time == false) {
                    player_ptr->Set_stop_move(false);
                    stop_move_one_time = true;
                }
                if (Engine::GetInput().PressAnyKey()) {
                    book2_clear = true;
                    Engine::GetGameStateManager().SetNextGameState(static_cast<int>(States::Libarary));
                }
            }
        }
        break;
    }

}

void Book2::Draw()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
    GetGSComponent<Background>()->Draw(*GetGSComponent<CameraComponent>());

    GetGSComponent<GameObjectManager>()->DrawAll(GetGSComponent<CameraComponent>()->Getworld_to_ndc());
    GetGSComponent<UIManager>()->AllDraw();
    switch (pattern) {
    case 0: /////////////////////////////////////////// Ready to Start ///////////////////////////////////////////
        quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("Are you ready?"));

        quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A1:"));
        quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A2:"));
        quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A3:"));
        quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A4:"));

        quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("Hit a Sphinx to start!"));

        break;

    case 1://///////////////////////////////////////// Quiz 1 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz1.getSentences(0)));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz1.getSentences(1)));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz1.getSentences(2)));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz1.getSentences(3)));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz1.getSentences(4)));
        }
        quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(std::to_string(GetGSComponent <DownTimer>()->RemainingInt())));
        break;

    case 2://///////////////////////////////////////// Ready to Quiz2 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("hmm... cool"));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A1: N"));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A2: E"));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A3: X"));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A4: T"));

            quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(" "));
        }
        break;

    case 3://///////////////////////////////////////// Quiz2 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz2.getSentences(0)));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz2.getSentences(1)));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz2.getSentences(2)));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz2.getSentences(3)));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz2.getSentences(4)));
        }
        quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(std::to_string(GetGSComponent <DownTimer>()->RemainingInt())));
        break;

    case 4://///////////////////////////////////////// Ready to Quiz3 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("You Got Z++"));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A1: N"));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A2: E"));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A3: X"));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A4: T"));

            quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(" "));
        }
        break;

    case 5://///////////////////////////////////////// Quiz3 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->SetFontSize(vec2(Engine::Instance()->Get_Window().x, Engine::Instance()->Get_Window().y / 5.0f));
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz3.getSentences(0)));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz3.getSentences(1)));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz3.getSentences(2)));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz3.getSentences(3)));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz3.getSentences(4)));
        }
        quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(std::to_string(GetGSComponent <DownTimer>()->RemainingInt())));
        break;

    case 6://///////////////////////////////////////// Ready to Quiz4 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->SetFontSize(vec2(Engine::Instance()->Get_Window().x / 1.5f, Engine::Instance()->Get_Window().y / 5.0f));
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("So nice!!"));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A1: G"));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A2: O"));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A3: O"));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A4: D"));

            quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(" "));
        }
        break;

    case 7://///////////////////////////////////////// Quiz4 ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz4.getSentences(0)));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz4.getSentences(1)));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz4.getSentences(2)));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz4.getSentences(3)));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(quiz4.getSentences(4)));
        }
        quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(std::to_string(GetGSComponent <DownTimer>()->RemainingInt())));
        break;

    case 8://///////////////////////////////////////// END ///////////////////////////////////////////
        if (draw_one_time == true) {
            quiz_sentence->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("You are God of Math!! clear!"));

            quiz_answer1->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A1: G"));
            quiz_answer2->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A2: O"));
            quiz_answer3->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A3: D"));
            quiz_answer4->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture("A4: S"));

            if (sphinx_ptr->SphinxHelth() == sphinx_maxlife - (pattern_life * 5)) {
                quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(" "));
            }
            else {
                quiz_cooltime->GetGOComponent<Sprite>()->Set_texture(Engine::Instance()->GetFont(0).PrintToTexture(" "));
            }
        }
        break;
    }

}

void Book2::Unload()
{
    if (SoundFlag) {
        SoundEffect::Stop_Loop_SoundEffect(SFX::Book2BGM);
        SoundEffect::StopSoundEffect(SFX::Death);
        SoundEffect::StopSoundEffect(SFX::ClearBGM);
        SoundFlag = false;
    }

    GetGSComponent<GameObjectManager>()->Unload();//
    ClearGSComponents();//
    SoundEffect::EraseSoundEffect();
}