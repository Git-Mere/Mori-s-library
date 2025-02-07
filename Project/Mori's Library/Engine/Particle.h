#pragma once
#include "../Engine/GameObject.h"
#include "../Mori/GameObjectTypes.h"
#include "../Engine/Engine.h"
#include "../Engine/GameObjectManager.h"
#include "../Mori/Player.h"
#include "../Engine/Constant.h"

class Particle : public GameObject {
public:
    Particle(const std::filesystem::path& sprite_file);
    void Start(vec2 p_position, vec2 p_velocity, double max_life);
    void Update(float dt) override;
    void Draw(const mat3& world_to_ndc) override;
    virtual bool CanCollideWith(GameObjectTypes other_object_type);
    bool Alive() {
        return life > 0;
    }
    void Set_Rotation(float value);
    GameObjectTypes Type() { return GameObjectTypes::Particle; }


private:
    double life;
};

template<typename T>
class ParticleManager : public Component {
public:
    ParticleManager();
    ~ParticleManager();
    void Emit(int count, vec2 emitter_position, vec2 emitter_velocity, vec2 direction, float spread);
    void Shot(int count, vec2 emitter_position, vec2 emitter_velocity);
    void Rotate_Shot(int count, vec2 position, vec2 velocity, float angle);
    void Show(int count, vec2 position, float angle);
    void SetPtr(void* value);
    void Inputindex();
private:
    std::vector<T*> particles;
    int index;
    float time_control;
    void* ptr;
    int number;
};

template<typename T>
inline ParticleManager<T>::ParticleManager() : time_control(0.0f)
{
    int maxCount = T::MaxCount;
    particles.reserve(maxCount);

    for (int i = 0; i < maxCount; i++) {
        T* particle = new T;  // T 타입의 입자 객체 생성
        //particles[index]->SetPtr(player);

        particles.push_back(particle);
        Engine::GetGameStateManager().GetGSComponent<GameObjectManager>()->Add(particle);
    }
    index = 0;
}

template<typename T>
inline ParticleManager<T>::~ParticleManager()
{
    particles.clear();
}

template<typename T>
void ParticleManager<T>::Emit(int count, vec2 emitter_position, vec2 emitter_velocity, vec2 direction, float spread) {
    for (int i = 0; i < count; i++) {
        //if (T::Alive()) {
        //    Engine::GetLogger().LogError("Particle overwritten");
        //}

        float angle_variation = 0.0;
        if (spread != 0) {
            angle_variation = ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2.0f;
        }
        vec2 random_magnitude = direction * (((rand() % 1024) / 2048.0f) + 0.5f);

        vec2 particle_velocity = Math::RotationMatrix(angle_variation) * random_magnitude + emitter_velocity;
        particles[index]->Start(emitter_position, particle_velocity, T::MaxLife);
        particles[index]->Set_Rotation(angle_variation);

        index++;
        if (index < 0 || index >= T::MaxCount) { index = 0; }
    }
}

template<typename T>
void ParticleManager<T>::Shot(int count, vec2 position, vec2 velocity) {
    for (int i = 0; i < count; i++) {
        particles[index]->Start(position, velocity - vec2(i * 30.0f), T::MaxLife);

        index++;
        if (index < 0 || index >= T::MaxCount) { index = 0; }
    }
}

template<typename T>
void ParticleManager<T>::Rotate_Shot(int count, vec2 position, vec2 velocity, float angle) {
    for (int i = 0; i < count; i++) {
        particles[index]->Start(position, velocity - vec2(i * 30.0f), T::MaxLife);
        particles[index]->Set_Rotation(angle);

        index++;
        if (index < 0 || index >= T::MaxCount) { index = 0; }
    }
}

template<typename T>
inline void ParticleManager<T>::SetPtr(void* value)
{
    for (int i = 0; i < T::MaxCount; i++) {
        if (particles[i] != nullptr) {
            particles[i]->ptr = std::move(value);
        }
    }
}

template<typename T>
void ParticleManager<T>::Show(int count, vec2 position, float angle) {
    for (int i = 0; i < count; i++) {
        particles[index]->Start(position, vec2(0, 0), T::MaxLife);
        particles[index]->Set_Rotation(angle);

        index++;
        if (index < 0 || index >= T::MaxCount) { index = 0; }
    }
}

template<typename T>
inline void ParticleManager<T>::Inputindex()
{
    for (int i = 0; i < T::MaxCount; i++) {
        particles[i]->number = i;
    }
}