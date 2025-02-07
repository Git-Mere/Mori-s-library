#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/CameraCom.h"
#include "../Mori/Player.h"

enum class UITypes {
    SkillUI,
    Life,
    ScreenEffect
};

class UI {
public:
    UI() {};
    virtual ~UI() {
        if (sprite != nullptr) {
            delete sprite;
        }
    };
    UI(std::string sprite_path) : sprite(new Sprite(sprite_path)) {};
    virtual void Update([[maybe_unused]]float dt) {};
    virtual void Draw([[maybe_unused]] const mat3& camera_matrix) {};
    virtual UITypes Type() = 0;

    vec2 Get_size() const { return size; } ;
    vec2 Get_position() const { return position; };
    mat3 Get_matrix() const { return matrix; };

protected:
    vec2 size;
    vec2 position;
    mat3 matrix;
    Sprite* sprite = nullptr;
};


class UIManager : public Component{
public:
    UIManager(const mat3& camera, Player& player);
    ~UIManager();
    void Add(UI* ui);
    void Update(float dt);
    void AllDraw();
    void SE_setting(Engine::ShaderName name, float f, bool depar = true);
    template<typename T>
    T* GetObject() {
        for (UI* ui : UIs) {
            T* ptr = dynamic_cast<T*>(ui);
            if (ptr != nullptr) {
                return ptr;
            }
        }
        return nullptr;
    }
private:
    std::vector<UI*> UIs;
    Player* player_;
    mat3 dont_move_camera_matrix;

};