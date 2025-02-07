#include "Electronic_wave.h"
#include "../Engine/Collision.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"
#include "../Engine/Particels.h"
#include "Bard.h"
#include <iostream>
namespace Particles {
    Electronic::Electronic() : Particle("assets/sprite/Bible/Spark.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(2000, 500));
        SetCollisionScale(vec2{ 2000,250 }, true);
        number = 0;
        ptr = nullptr;
    };

    bool Electronic::CanCollideWith(GameObjectTypes other_object_type) {
        if (other_object_type == GameObjectTypes::Player) {
            return true;
        }
        return false;
    };

    Electronic_line::Electronic_line() : Particle("assets/sprite/Bible/Light_spear.spt") {
        SetPosition(vec2(-5000, -5000));
        SetScale(vec2(2000, 500));
        SetCollisionScale(vec2{ 2000,300 }, true);
        number = 0;
        ptr = nullptr;
    };

    //bool Electronic_line::CanCollideWith(GameObjectTypes other_object_type) {
    //    if (other_object_type == GameObjectTypes::Player) {
    //        return true;
    //    }
    //    return false;
    //};
}