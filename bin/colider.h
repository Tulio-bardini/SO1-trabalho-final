#ifndef colider_H
#define colider_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "ship.h"
#include "enemyPurple.h"
#include "Laser.h"

__BEGIN_API

class Colider
{

public:
    Colider(Ship *ship, std::list<std::shared_ptr<EnemyPurple>> *enemies, bool *finish);
    ~Colider();
    void run();
    void checkCollisionOnEnemies();

private:
    Ship *_ship;
    std::list<std::shared_ptr<EnemyPurple>> *_enemies;
    bool *_finish;
};

__END_API

#endif