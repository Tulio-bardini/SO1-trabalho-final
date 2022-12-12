/**
 * @file Mine.cc
 * @brief Implementation of Mine class- draws, updates and destroys enemies
 *
 * @author
 * @bug
 **/
#include "Mine.h"

__BEGIN_API

#define MINE_SIZE 20
#define TypeClassNumber 2

Mine::Mine(float *dt, std::list<Laser> *lasers, bool * finish)
{
    speed = Vector(-80, 0);
    _dt = dt;
    centre.rollRandom();
    _finish = finish;
    _lasers = lasers;
    size = MINE_SIZE;

    _explodeTimer = std::make_shared<Timer>(60);
    _explodeTimer->create();
    _explodeTimer->startTimer();
    
    loadSprite();
}

Mine::~Mine()
{
}

void Mine::update()
{
    centre = centre + speed * (*_dt);

    if (_explodeTimer->getCount() > 130 && stage < 4)
    {
        stage += 1;
        if (stage >= 3)
        {
            explode();
        }
        _explodeTimer->srsTimer();
    }
}

void Mine::draw()
{
    if(!dead) {
        mineSprite->draw_region(stage, 3-life, 40.0, 41.0, centre, 0);
    }

}

void Mine::hit(int damage)
{
    if (life > 0)
    {
        life -= damage;
    }
    else {
        dead = true;
    }
}

void Mine::explode()
{
    dead = true;

    // right side
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(112, 37)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(75, 75)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(37, 112)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-112, 37)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-75, 75)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-37, 112)));
    // left side
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(112, -37)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(75, -75)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(37, -112)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-112, -37)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-75, -75)));
    _lasers->push_back(Laser(centre, al_map_rgb(200, 0, 0), TypeClassNumber, _dt, Vector(-37, -112)));
}

void Mine::loadSprite()
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, "resources");
    al_change_directory(al_path_cstr(path, '/'));

    mineSprite = std::make_shared<Sprite>("spikebomb.png");
}

void Mine::runMine(std::list<std::shared_ptr<Mine>> *mines, bool *finish)
{
    while (!*finish)
    {
        if (!mines->empty())
        {
            for (auto it = mines->begin();
                 it != mines->end(); ++it)
            {
                if (!(*it)->dead) {
                    (*it)->update();
                }
            }
            std::list<std::shared_ptr<Mine>> newMines;
            for (auto it = mines->begin(); it != mines->end(); ++it)
            {
                if (!(*it)->dead)
                {
                    newMines.push_back(*it);
                }
            }
            mines->clear();
            mines->assign(newMines.begin(), newMines.end());
        }
        
        Thread::yield();
    }

    Thread::running()->thread_exit(0);
}

__END_API