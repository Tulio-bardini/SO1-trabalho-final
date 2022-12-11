/**
 * @file Mine.cc
 * @brief Implementation of Mine class- draws, updates and destroys enemies
 *
 * @author
 * @bug
 **/
#include "Mine.h"

__BEGIN_API

#define Mine_SIZE 20

Mine::Mine(float *dt, bool * finish)
{
    speed = Vector(-80, 0);
    _dt = dt;
    centre = Point(800, 300);
    _finish = finish;

    _explodeTimer = std::make_shared<Timer>(60);
    _explodeTimer->create();
    _explodeTimer->startTimer();

    lasersThread = new Thread(Laser::runLaser, &lasers, _finish);

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
            stage = 2;
            explode();
            dead = true;
        }
        _explodeTimer->srsTimer();
    }
}

void Mine::draw()
{
    drawLaser();
    mineSprite->draw_region(stage, 3-life, 40.0, 41.0, centre, 0);
}

void Mine::hit()
{
    if (life > 0)
    {
        life -= 1;
    }
}

void Mine::drawLaser() {
    if (!lasers.empty())
    {
        for (std::list<Laser>::iterator it = lasers.begin();
            it != lasers.end(); ++it)
        {
            it->draw();
        }
    }
}

void Mine::explode()
{
    // right side
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(75, 25)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(50, 50)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(25, 75)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(-75, 25)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(-50, 50)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(-25, 75)));
    // left side
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(75, -25)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(50, -50)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(25, -75)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(-75, -25)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(-50, -50)));
    lasers.push_back(Laser(centre, al_map_rgb(200, 0, 0), Vector(-25, -75)));
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
                (*it)->update();
            }
            std::list<std::shared_ptr<Mine>> newMines;
            for (auto it = mines->begin(); it != mines->end(); ++it)
            {
                if (!(*it)->dead)
                {
                    newMines.push_back(*it);
                }
            }
        }
        Thread::yield();
    }

    Thread::running()->thread_exit(0);
}

__END_API