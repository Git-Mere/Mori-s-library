#include "Final_Spark.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Constant.h"
#include "Lucifer.h"
#include "Player.h"
#include "../ScreenEffect.h"

namespace Particles {
    FinalSpark::FinalSpark() : Particle("assets/sprite/Bible/Spark.spt") {
        SetScale(vec2(1500, 200));//
        SetPosition(vec2(-5000, -5000));
        SetCollisionScale(vec2{ 1500, 90 }, true);//
        current_state = &state_ready;
        current_state->Enter(this);
    };
    bool FinalSpark::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void FinalSpark::State_Ready::Enter(GameObject* object) {
        FinalSpark* spark = static_cast<FinalSpark*>(object);
        timer = 0;
        spark->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
        spark->number;
    }

    void FinalSpark::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        FinalSpark* spark = static_cast<FinalSpark*>(object);
        timer += dt;
        switch (spark->number) {
        case 0:
            spark->theta = 0;
            break;
        case 1:
            spark->theta = 0.5;
            break;
        case 2:
            spark->theta = 1.0;
            break;
        case 3:
            spark->theta = 1.5;
            break;
        }
    }

    void FinalSpark::State_Ready::CheckExit([[maybe_unused]]GameObject* object) {
        FinalSpark* spark = static_cast<FinalSpark*>(object);
        if (timer > 1.5) {
            spark->change_state(&spark->state_run);
        }
    }

    void FinalSpark::State_Run::Enter([[maybe_unused]] GameObject* object) {

        Engine::Instance()->GetGameStateManager().GetGSComponent<effect::Shake>()->Set_info(7.5f, 5, 10, 1);
        timer = 0;
    }

    void FinalSpark::State_Run::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        FinalSpark* spark = static_cast<FinalSpark*>(object);
        timer += dt;
        if (spark->Alive() && timer <= 7.5) {
            spark->theta -= dt/3;
            vec2 boss_pos = reinterpret_cast<Lucifer*>(spark->ptr)->GetPosition();

            float orbitRadius = 750.0f;
            float orbitX = boss_pos.x + orbitRadius * cos(spark->theta * Math::PI);
            float orbitY = boss_pos.y + orbitRadius * sin(spark->theta * Math::PI);

            spark->SetRotation(spark->theta * Math::PI);
            spark->SetPosition({ orbitX, orbitY });
        }
    }

    void FinalSpark::State_Run::CheckExit([[maybe_unused]] GameObject* object) {//
        FinalSpark* spark = static_cast<FinalSpark*>(object);
        if (timer >= 8.5) {
            spark->change_state(&spark->state_ready);
        }
    }

/////////////////////////////////////////////////////// Dark Spark ///////////////////////////////////////////////////////

    DarkSpark::DarkSpark() : Particle("assets/sprite/Bible/Dark_Spark.spt") {
        SetScale(vec2(1500, 200));
        SetPosition(vec2(-5000, -5000));
        SetCollisionScale(vec2{ 1500, 90 }, true);
        current_state = &state_ready;
        current_state->Enter(this);
    };
    bool DarkSpark::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    void DarkSpark::State_Ready::Enter(GameObject* object) {
        DarkSpark* spark = static_cast<DarkSpark*>(object);
        timer = 0;
        spark->GetGOComponent<Sprite>()->PlayAnimation(static_cast<int>(Animations::Ready));
        spark->number;
        //spark->is_collision = false;
    }

    void DarkSpark::State_Ready::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        DarkSpark* spark = static_cast<DarkSpark*>(object);
        timer += dt;
        switch (spark->number) {
        case 0:
            spark->theta = 0;
            break;
        case 1:
            spark->theta = 0.25;
            break;
        case 2:
            spark->theta = 0.5;
            break;
        case 3:
            spark->theta = 0.75;
            break;
        case 4:
            spark->theta = 1.0;
            break;
        case 5:
            spark->theta = 1.25;
            break;
        case 6:
            spark->theta = 1.5;
            break;
        case 7:
            spark->theta = 1.75;
            break;
        }

        //if (timer > 1.0 && spark->is_collision == false) {
        //    spark->is_collision = true;
        //}
    }

    void DarkSpark::State_Ready::CheckExit([[maybe_unused]] GameObject* object) {
        DarkSpark* spark = static_cast<DarkSpark*>(object);
        if (timer > 1.5) {
            spark->change_state(&spark->state_run);
        }
    }

    void DarkSpark::State_Run::Enter([[maybe_unused]] GameObject* object) {
        timer = 0;
    }

    void DarkSpark::State_Run::Update([[maybe_unused]] GameObject* object, [[maybe_unused]] float dt) {
        DarkSpark* spark = static_cast<DarkSpark*>(object);
        timer += dt;
        if (spark->Alive() && timer <= 7.5) {
            spark->theta -= dt / 5;
            vec2 boss_pos = reinterpret_cast<Lucifer*>(spark->ptr)->GetPosition();

            float orbitRadius = 750.0f;
            float orbitX = boss_pos.x + orbitRadius * cos(spark->theta * Math::PI);
            float orbitY = boss_pos.y + orbitRadius * sin(spark->theta * Math::PI);

            spark->SetRotation(spark->theta * Math::PI);
            spark->SetPosition({ orbitX, orbitY });
        }
    }

    void DarkSpark::State_Run::CheckExit([[maybe_unused]] GameObject* object) {
        DarkSpark* spark = static_cast<DarkSpark*>(object);
        if (timer >= 8.5) {
            spark->change_state(&spark->state_ready);
        }
    }

/////////////////////////////// Lazer /////////////////////////////// 
    Lazer::Lazer() : Particle("assets/sprite/Bible/Lazer.spt") {
        SetScale(vec2(1500, 200));
        SetPosition(vec2(-5000, -5000));
        SetCollisionScale(vec2{ 1500, 90 }, true);
        current_state->Enter(this);
    };
    bool Lazer::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };
}