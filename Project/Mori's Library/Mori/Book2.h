#pragma once
#include "../Engine/GameState.h"
#include <Random.h>
#include <string>
#include "TextRead.h"

class Player;
class QuizBox;
class FontObject;
class Block;
class MathPerson;
class Sphinx;
class GameOver;
class PressAnyKey;
class Screen_Effect;
class GameClear;

class Book2 : public GameState {
public:
    void Load() override;
    void Update([[maybe_unused]] float dt) override;
    void Unload() override;
    void Draw() override;

    std::string GetName() override { return "Book2"; }

private:
    Player* player_ptr;
    MathPerson* person_ptr;
    Sphinx* sphinx_ptr;

    int quiz_number = 0;
    float window_x = Engine::Instance()->Get_Window().x;
    float window_y = Engine::Instance()->Get_Window().y;
    bool draw_one_time = false;
    //answer
    QuizBox* quizBox1;
    QuizBox* quizBox2;
    QuizBox* quizBox3;
    QuizBox* quizBox4;

    bool answer1 = false;
    bool answer2 = false;
    bool answer3 = false;
    bool answer4 = false;
    int now_answer = 0;

    // pattern sequence
    float time_store = 0;
    float count10 = 10;
    int pattern = 0;
    bool small_pattern = true;

    //quiz
    int check_answer(std::string number) {
        if (number == "1") {
            return 1;
        }
        else if (number == "2") {
            return 2;
        }
        else if (number == "3") {
            return 3;
        }
        else if (number == "4") {
            return 4;
        }
        else {
            return 10;
        }
    }

    FontObject* quiz_sentence;
    FontObject* quiz_answer1;
    FontObject* quiz_answer2;
    FontObject* quiz_answer3;
    FontObject* quiz_answer4;
    GameOver* gameover;
    PressAnyKey* pressanykey;
    Screen_Effect* darkscreen;
    GameClear* gameclear;

    FontObject* quiz_cooltime;
    float save_time = 0.0f;
    static constexpr float quiz_time_count = 10;

    int random_quiz_number1 = util::random(1, 6);
    Sentence quiz1 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz1-" + std::to_string(random_quiz_number1) + ".txt");
    int pattern1_answer = check_answer(quiz1.getSentences(5));

    int random_quiz_number2 = util::random(1, 6);
    Sentence quiz2 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz2-" + std::to_string(random_quiz_number1) + ".txt");
    int pattern2_answer = check_answer(quiz2.getSentences(5));

    int random_quiz_number3 = util::random(1, 6);
    Sentence quiz3 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz3-" + std::to_string(random_quiz_number1) + ".txt");
    int pattern3_answer = check_answer(quiz3.getSentences(5));

    int random_quiz_number4 = util::random(1, 6);
    Sentence quiz4 = loadQuizSentencesFromFile<Sentence>("assets/textfile/Quiz4-" + std::to_string(random_quiz_number1) + ".txt");
    int pattern4_answer = check_answer(quiz4.getSentences(5));

    Block* block_ptr;
    vec2 block_position[4] = { vec2(-window_x / 4,  window_y / 4),  //quiz1
                               vec2( window_x / 4,  window_y / 4),  //quiz2
                               vec2(-window_x / 4, -window_y / 4),  //quiz3
                               vec2( window_x / 4, -window_y / 4)}; //quiz4

    //magicball
    int magicball_count = 0;
    int max_magicball = 50;

    //sphinx
    vec2 sphinx_position = vec2{ 0, 300 };
    const int sphinx_maxlife = 15;
    int pattern_life = 3;
    vec2 random_position[4] = { vec2{ 0, 300 }, vec2{500, 0}, vec2{0, -300}, vec2{-500 , 0}, };

    bool SoundFlag = false;
    bool Sound_One_Time = false;
    float dead_cooltime = 0;
    float dead_cooltime_max = 1;
    bool stop_move_one_time = false;
    float background_sound;
};