/**
 * @file Boss.h
 * @brief derived class of Enemy
 *
 * @author
 * @bugs lots
 *
 **/

#ifndef BOSS_H
#define BOSS_H

// #include "Updateable.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include "Laser.h"
#include "ship.h"
#include "Missile.h"

__BEGIN_API

class Boss
{

public:
    Boss(Ship * ship, std::list<std::shared_ptr<Missile>> *missiles , float *dt, std::list<Laser> *lasers, bool *finish);
    ~Boss();

    Point centre;
    Vector speed;
    bool dead = false;
    int size;

    void fireLaser1();
    void fireLaser2();
    void fireMissile();

    void restart();
    void update();
    void run();
    void load_sprite();
    void deathAnim();
    void hit();
    void chooseFrame();
    void draw();
    void setBossAlive();
    void setTimers();
    bool getBossAlive();

private:
    // timer
    std::shared_ptr<Timer> fireLaserDelay1;
    std::shared_ptr<Timer> fireLaserDelay2;
    std::shared_ptr<Timer> fireMissileDelay;
    // lists
    std::list<Laser> * _lasers;
    Ship * _ship;
    std::list< std::shared_ptr<Missile> > * _missiles;

    // general
    int lives;
    int dAnim;
    float *_dt;
    bool *_finish;
    // flags
    bool dAnim_complete;
    bool aliveBoss = false;
    bool targetable;
    std::shared_ptr<Sprite> bossSprite;
    std::shared_ptr<Sprite> bossDeath;
    // for animation
    int col, row;
};

__END_API

#endif
