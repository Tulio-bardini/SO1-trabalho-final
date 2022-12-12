/**
 * @file EnemyPurple.cc
 * @brief Implementation of EnemyPurple class- draws, updates and destroys enemies
 *
 * @author
 * @bug
 **/
#include "enemyPurple.h"

__BEGIN_API

#define EnemyPurple_SIZE 20
#define TypeClassNumber 2

EnemyPurple::EnemyPurple(Point cen, Vector spd, std::list<Laser> *lasers, float *dt, bool *finish)
{
    speed = spd;
    color = al_map_rgb(246, 64, 234);
    centre = cen;
    _dt = dt;
    size = EnemyPurple_SIZE;
    _finish = finish;
    _lasers = lasers;

    _fireTimer = std::make_shared<Timer>(60);
    _fireTimer->create();
    _fireTimer->startTimer();

    // lasersThread = new Thread(Laser::runLaser, &lasers, _finish);

    loadSprite();
    load_assets();
}

// creates and starts the fireDelay timer for the EnemyPurple
void EnemyPurple::load_assets()
{
    fireDelay = std::make_shared<Timer>(60);
    fireDelay->create();
    fireDelay->startTimer();
}

EnemyPurple::~EnemyPurple()
{
    fireDelay.reset();
}

// explode
void EnemyPurple::hit()
{
    dead = true;
}

// draw image to display of enemy ship
void EnemyPurple::draw()
{
    if (!dead)
    {
        enemySprite->draw_tinted(centre, color, 0);
    }
    else
    {
        // enemy has been hit and killed, proceed through death animation sequence
        if (dAnim < 5)
            deathAnim();
        else
            dAnim_complete = true;
    }
}

void EnemyPurple::deathAnim()
{
    deathSprite->draw_death_anim(dAnim, centre, 0);
    dAnim++; // move forward to the next frame of the animation for the next call
}

void EnemyPurple::loadSprite()
{
    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_append_path_component(path, "resources");
    al_change_directory(al_path_cstr(path, '/'));

    enemySprite = std::make_shared<Sprite>("EnemyBasic.png"); // espaçonave do enimigo
    deathSprite = std::make_shared<Sprite>("explode.png");    // animação explosão
}

void EnemyPurple::fire()
{
    _lasers->push_back(Laser(centre, color, TypeClassNumber, _dt, Vector(-330, 50)));
    _lasers->push_back(Laser(centre, color, TypeClassNumber, _dt, Vector(-330, -50)));
}

// update position of enemy ships
void EnemyPurple::update()
{

    centre = centre + speed * (*_dt);

    if (centre.x < 0)
    {
        dead = true;
    }

    if (_fireTimer->getCount() > 150 && !dead)
    {
        fire();
        _fireTimer->srsTimer();
    }

    if (centre.y > 600 - EnemyPurple_SIZE && speed.y > 0)
        speed.reflectY();
    if (centre.y < 0 - EnemyPurple_SIZE && speed.y < 0)
        speed.reflectY();
}

void EnemyPurple::runEnemies(std::list<std::shared_ptr<EnemyPurple>> *enemyList, bool *finish)
{
    while (!*finish)
    {
        if (!enemyList->empty())
        {
            for (auto it = enemyList->begin();
                 it != enemyList->end(); ++it)
            {
                if (!(*it)->dead)
                {
                    (*it)->update();
                }
            }
            std::list<std::shared_ptr<EnemyPurple>> newEnemyList;
            for (auto it = enemyList->begin(); it != enemyList->end(); ++it)
            {
                if (!(*it)->dAnim_complete)
                {
                    newEnemyList.push_back(*it);
                }
            }
            enemyList->clear();
            enemyList->assign(newEnemyList.begin(), newEnemyList.end());
        }
        Thread::yield();
    }
    Thread::running()->thread_exit(0);
}

__END_API