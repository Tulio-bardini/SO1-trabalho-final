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
#include "Mine.h"
#include "Missile.h"

__BEGIN_API

class Colider
{

public:
    Colider(Ship *ship, std::list<std::shared_ptr<EnemyPurple>> *enemies,
             std::list<Laser> *lasers, std::list< std::shared_ptr<Mine> > *mines, 
             std::list< std::shared_ptr<Missile> > *missiles, bool *finish);
    ~Colider();
    void run();
    void checkCollisionOnEnemies();
    void checkCollisionOnPlayer();
    void checkCollisionOnMines();

private:
    Ship *_ship;
    std::list<std::shared_ptr<EnemyPurple>> *_enemies;
    std::list<Laser> *_lasers;
    std::list< std::shared_ptr<Mine> > *_mines;
    std::list< std::shared_ptr<Missile> > *_missiles;
    bool *_finish;
};

__END_API

#endif