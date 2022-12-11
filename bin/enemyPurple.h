/**
 * @file enemyPurple.h
 *
 * @author
 * @bug
 **/

#ifndef ENEMYPURPLE_H
#define ENEMYPURPLE_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <memory>
#include <string>
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include "Laser.h"

__BEGIN_API

class EnemyPurple
{

public:
    EnemyPurple(Point p, Vector s, std::list<Laser> *lasers, float *dt, bool *finish);
    ~EnemyPurple();

    Point centre;
    ALLEGRO_COLOR color;
    Vector speed;
    bool dead = false;
    int size;

    static void runEnemies(std::list<std::shared_ptr<EnemyPurple>> *enemyList, bool *finish);

    void update();
    void load_assets();
    void loadSprite();
    void deathAnim();
    void hit();
    void draw();
    void fire();

private:
    std::shared_ptr<Timer> fireDelay;
    std::shared_ptr<Sprite> enemySprite;
    std::shared_ptr<Sprite> deathSprite;
    std::shared_ptr<Timer> _fireTimer;
    std::list<Laser> * _lasers;
    Vector projSpeed;
    int fireSpeed;
    int lives;
    int dAnim;
    bool dAnim_complete;
    float *_dt;
    bool *_finish;
    
};

__END_API

#endif