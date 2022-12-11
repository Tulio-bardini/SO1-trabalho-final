#include "colider.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>

#include "Timer.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "ship.h"
#include "Laser.h"

__BEGIN_API

Colider::Colider(Ship *ship, std::list<std::shared_ptr<EnemyPurple>> *enemies, bool *finish)
{
    _ship = ship;
    _enemies = enemies;
    _finish = finish;
}

Colider::~Colider()
{
}

void Colider::run() {
    while (!*_finish)
    {
        checkCollisionOnEnemies();
        Thread::yield();
    }

    Thread::running()->thread_exit(0);
}

void Colider::checkCollisionOnEnemies()
{
    if (!_ship->lasers.empty() && !_enemies->empty() && _ship)
    {
        for (std::list<Laser>::iterator it_laser = _ship->lasers.begin();
             it_laser != _ship->lasers.end(); ++it_laser)
        {

            Point pt_laser = it_laser->centre;
            for (std::list< std::shared_ptr<EnemyPurple> >::iterator it_enemP =
                     _enemies->begin();
                 it_enemP != _enemies->end(); ++it_enemP)
            {

                // set bounding points
                Point pt_enemP = (*it_enemP)->centre;
                int enem_sizeP = (*it_enemP)->size;

                if (!(*it_enemP)->dead){
                    // check for collision
                    if ((pt_laser.x > pt_enemP.x - enem_sizeP) &&
                        (pt_laser.x < pt_enemP.x + enem_sizeP) &&
                        (pt_laser.y > pt_enemP.y - enem_sizeP) &&
                        (pt_laser.y < pt_enemP.y + enem_sizeP))
                    {
                        // register damage on enemy and flag projectile as dead
                        it_laser->live = false;
                        (*it_enemP)->hit();
                    }
                }
            }
        }
    }
}

__END_API
