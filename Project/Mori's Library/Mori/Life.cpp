#include "Life.h"

Life::Life(int life_number, bool is_player) : life_number(life_number), for_player(is_player)
{
    vec2 tmp = Engine::Instance()->Get_Window() / 2;
    size = { tmp.x / 2, tmp.x / 2 };
    position = { -tmp.x + 70, tmp.y - 50 };
    if (is_player == true) {
        for (int i = 0; i < life_number; i++) {
            hearts.emplace_back(new Sprite("assets/sprite/Player/Life.spt"));
        }
    }
};

Life::~Life()
{
    for (auto heart : hearts) {
        delete heart;
    }
    hearts.clear();
}

void Life::Update(float dt)
{
    if (for_player) {
        for (auto heart : hearts) {
            heart->Update(dt);
        }
    }
}

void Life::Draw(const mat3& camera_matrix)
{
    if (for_player) {
        size_t vec_size = hearts.size();
        for (int i = 0; i < vec_size; i++) {
            vec2 final_pos = vec2{ position.x + (i * size.x / 4) , position.y };
            const mat3 T = mat3::build_translation(final_pos);
            const mat3 H = mat3::build_scale(size);

            mat3 model_to_ndc = T * H;
            mat3 final_matrix = camera_matrix * model_to_ndc;

            hearts[i]->No_Gameobject_Draw(final_matrix);
        }
    }
}


bool Life::Dead()
{
    return life_number <= 0;
}

void Life::Sub([[maybe_unused]] int how)
{
    if (for_player) {
        hearts[life_number - 1]->PlayAnimation(Ani::dis_effect);
    }
    life_number -= how;
}

void Life::Add(int how)
{
    life_number += how;
}

int Life::GetLifeNumber() {
    return life_number;
}
