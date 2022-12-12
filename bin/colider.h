#ifndef colider_H
#define colider_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>

#include "Timer.h"
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
#include "Boss.h"

__BEGIN_API

class Colider
{

public:
    Colider(Ship *ship, std::list<std::shared_ptr<EnemyPurple>> *enemies,
             std::list<Laser> *lasers, std::list< std::shared_ptr<Mine> > *mines, 
             std::list< std::shared_ptr<Missile> > *missiles,
             Boss * boss, bool *finish);
    ~Colider();
    void run();
    void checkCollisionOnEnemies();
    void checkCollisionOnPlayer();
    void checkCollisionOnMines();
    void checkCollisionOnBoss();

private:
    Ship *_ship;
    std::list<std::shared_ptr<EnemyPurple>> *_enemies;
    std::list<Laser> *_lasers;
    std::list< std::shared_ptr<Mine> > *_mines;
    std::list< std::shared_ptr<Missile> > *_missiles;
    Boss * _boss;
    bool *_finish;
};

__END_API

#endif