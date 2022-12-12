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
#include "Missile.h"

__BEGIN_API

Colider::Colider(Ship *ship, std::list<std::shared_ptr<EnemyPurple>> *enemies,
                 std::list<Laser> *lasers, std::list<std::shared_ptr<Mine>> *mines,
                 std::list<std::shared_ptr<Missile>> *missiles,
                 Boss *boss, bool *finish)
{
    _ship = ship;
    _enemies = enemies;
    _finish = finish;
    _lasers = lasers;
    _mines = mines;
    _missiles = missiles;
    _boss = boss;
}

Colider::~Colider()
{
}

void Colider::run()
{
    while (!*_finish)
    {
        checkCollisionOnEnemies();
        checkCollisionOnPlayer();
        checkCollisionOnMines();
        checkCollisionOnBoss();
        Thread::yield();
    }

    Thread::running()->thread_exit(0);
}

void Colider::checkCollisionOnMines()
{

    if (!_lasers->empty() && !_mines->empty() && _ship)
    {
        for (std::list<std::shared_ptr<Mine>>::iterator it_mine =
                 _mines->begin();
             it_mine != _mines->end(); ++it_mine)
        {
            for (std::list<Laser>::iterator it_laser = _lasers->begin();
                 it_laser != _lasers->end(); ++it_laser)
            {
                if (it_laser->classOwner == 1)
                {
                    Point pt_laser = it_laser->centre;

                    // set bounding points
                    Point pt_mine = (*it_mine)->centre;
                    int mine_size = (*it_mine)->size;

                    if (!(*it_mine)->dead)
                    {
                        // check for collision
                        if ((pt_laser.x > pt_mine.x - mine_size) &&
                            (pt_laser.x < pt_mine.x + mine_size) &&
                            (pt_laser.y > pt_mine.y - mine_size) &&
                            (pt_laser.y < pt_mine.y + mine_size))
                        {
                            // register damage on enemy and flag projectile as dead
                            it_laser->live = false;
                            (*it_mine)->hit(1);
                        }
                    }
                }
            }
            for (std::list<std::shared_ptr<Missile>>::iterator it_missile =
                     _missiles->begin();
                 it_missile != _missiles->end(); ++it_missile)
            {
                if ((*it_missile)->classOwner == 1)
                {
                    // set bounding points
                    Point pt_mine = (*it_mine)->centre;
                    int mine_size = (*it_mine)->size;

                    if (!(*it_mine)->dead)
                    {
                        // check for collision
                        if (((*it_missile)->centre.x > pt_mine.x - mine_size) &&
                            ((*it_missile)->centre.x < pt_mine.x + mine_size) &&
                            ((*it_missile)->centre.y > pt_mine.y - mine_size) &&
                            ((*it_missile)->centre.y < pt_mine.y + mine_size))
                        {
                            // register damage on enemy and flag projectile as dead
                            (*it_mine)->hit(3);
                        }
                    }
                }
            }
        }
    }
}

void Colider::checkCollisionOnEnemies()
{
    if (!_lasers->empty() && !_enemies->empty() && _ship)
    {
        for (std::list<std::shared_ptr<EnemyPurple>>::iterator it_enemP =
                 _enemies->begin();
             it_enemP != _enemies->end(); ++it_enemP)
        {

            for (std::list<Laser>::iterator it_laser = _lasers->begin();
                 it_laser != _lasers->end(); ++it_laser)
            {
                if (it_laser->classOwner == 1)
                {
                    Point pt_laser = it_laser->centre;

                    // set bounding points
                    Point pt_enemP = (*it_enemP)->centre;
                    int enem_sizeP = (*it_enemP)->size;

                    if (!(*it_enemP)->dead)
                    {
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
            for (std::list<std::shared_ptr<Missile>>::iterator it_missile =
                     _missiles->begin();
                 it_missile != _missiles->end(); ++it_missile)
            {
                if ((*it_missile)->classOwner == 1)
                {
                    // set bounding points
                    Point pt_enemP = (*it_enemP)->centre;
                    int enem_sizeP = (*it_enemP)->size;

                    if (!(*it_enemP)->dead)
                    {
                        // check for collision
                        if (((*it_missile)->centre.x > pt_enemP.x - enem_sizeP) &&
                            ((*it_missile)->centre.x < pt_enemP.x + enem_sizeP) &&
                            ((*it_missile)->centre.y > pt_enemP.y - enem_sizeP) &&
                            ((*it_missile)->centre.y < pt_enemP.y + enem_sizeP))
                        {
                            // register damage on enemy and flag projectile as dead
                            (*it_enemP)->hit();
                        }
                    }
                }
            }
        }
    }
}

void Colider::checkCollisionOnPlayer()
{

    if (_ship)
    {
        for (std::list<Laser>::iterator it_laser = _lasers->begin();
             it_laser != _lasers->end(); ++it_laser)
        {
            if (it_laser->classOwner == 2)
            {
                // set bounding points
                Point pt_laser = it_laser->centre;
                int player_size = _ship->size;

                // check for collision
                if ((pt_laser.x > _ship->centre.x - player_size) &&
                    (pt_laser.x < _ship->centre.x + player_size) &&
                    (pt_laser.y > _ship->centre.y - player_size) &&
                    (pt_laser.y < _ship->centre.y + player_size))
                {
                    // register damage on enemy and flag projectile as dead
                    it_laser->live = false;
                    _ship->hit(1);
                }
            }
        }

        for (std::list<std::shared_ptr<EnemyPurple>>::iterator it_enemP =
                 _enemies->begin();
             it_enemP != _enemies->end(); ++it_enemP)
        {
            int enem_sizeP = (*it_enemP)->size;
            if (!(*it_enemP)->dead)
            {
                if (((_ship->centre.x + _ship->size > (*it_enemP)->centre.x - enem_sizeP) &&
                     (_ship->centre.x + _ship->size < (*it_enemP)->centre.x + enem_sizeP) &&
                     (_ship->centre.y + _ship->size > (*it_enemP)->centre.y - enem_sizeP) &&
                     (_ship->centre.y + _ship->size < (*it_enemP)->centre.y + enem_sizeP)) ||
                    ((_ship->centre.x - _ship->size > (*it_enemP)->centre.x - enem_sizeP) &&
                     (_ship->centre.x - _ship->size < (*it_enemP)->centre.x + enem_sizeP) &&
                     (_ship->centre.y - _ship->size > (*it_enemP)->centre.y - enem_sizeP) &&
                     (_ship->centre.y - _ship->size < (*it_enemP)->centre.y + enem_sizeP)))
                {
                    (*it_enemP)->hit();
                    _ship->hit(1);
                }
            }
        }

        for (std::list<std::shared_ptr<Mine>>::iterator it_mine =
                 _mines->begin();
             it_mine != _mines->end(); ++it_mine)
        {
            int mine_size = (*it_mine)->size;
            if (!(*it_mine)->dead)
            {
                if (((_ship->centre.x + _ship->size > (*it_mine)->centre.x - mine_size) &&
                     (_ship->centre.x + _ship->size < (*it_mine)->centre.x + mine_size) &&
                     (_ship->centre.y + _ship->size > (*it_mine)->centre.y - mine_size) &&
                     (_ship->centre.y + _ship->size < (*it_mine)->centre.y + mine_size)) ||
                    ((_ship->centre.x - _ship->size > (*it_mine)->centre.x - mine_size) &&
                     (_ship->centre.x - _ship->size < (*it_mine)->centre.x + mine_size) &&
                     (_ship->centre.y - _ship->size > (*it_mine)->centre.y - mine_size) &&
                     (_ship->centre.y - _ship->size < (*it_mine)->centre.y + mine_size)))
                {
                    (*it_mine)->explode();
                    _ship->hit(1);
                }
            }
        }
        for (std::list<std::shared_ptr<Missile>>::iterator it_missile =
                 _missiles->begin();
             it_missile != _missiles->end(); ++it_missile)
        {
            if ((*it_missile)->classOwner == 2)
            {
                // set bounding points
                Point pt_ship = _ship->centre;
                int ship_size = _ship->size;

                if (!_ship->dead)
                {
                    // check for collision
                    if (((*it_missile)->centre.x > pt_ship.x - ship_size) &&
                        ((*it_missile)->centre.x < pt_ship.x + ship_size) &&
                        ((*it_missile)->centre.y > pt_ship.y - ship_size) &&
                        ((*it_missile)->centre.y < pt_ship.y + ship_size))
                    {
                        // register damage on enemy and flag projectile as dead
                        (_ship)->hit(1);
                    }
                }
            }
        }
    }
}

void Colider::checkCollisionOnBoss() {
    if (!_boss->dead) {
        for (std::list<Laser>::iterator it_laser = _lasers->begin();
             it_laser != _lasers->end(); ++it_laser)
        {
            if (it_laser->classOwner == 1)
            {
                // set bounding points
                Point pt_laser = it_laser->centre;
                int boss_size = _boss->size;

                // check for collision
                if ((pt_laser.x > _boss->centre.x - boss_size) &&
                    (pt_laser.x < _boss->centre.x + boss_size) &&
                    (pt_laser.y > _boss->centre.y - boss_size) &&
                    (pt_laser.y < _boss->centre.y + boss_size))
                {
                    // register damage on enemy and flag projectile as dead
                    it_laser->live = false;
                    _boss->hit();
                }
            }
        }
        for (std::list<std::shared_ptr<Missile>>::iterator it_missile =
                 _missiles->begin();
             it_missile != _missiles->end(); ++it_missile)
        {
            if ((*it_missile)->classOwner == 1)
            {
                // set bounding points
                Point pt_boss = _boss->centre;
                int boss_size = _boss->size;

                if (!_boss->dead)
                {
                    // check for collision
                    if (((*it_missile)->centre.x > pt_boss.x - boss_size) &&
                        ((*it_missile)->centre.x < pt_boss.x + boss_size) &&
                        ((*it_missile)->centre.y > pt_boss.y - boss_size) &&
                        ((*it_missile)->centre.y < pt_boss.y + boss_size))
                    {
                        // register damage on enemy and flag projectile as dead
                        (_boss)->hit();
                        (*it_missile)->live = false;
                    }
                }
            }
        }
    }
}

__END_API
