/**
 * @file Creep.h
 * @brief derived class of Enemy
 *
 * @author
 * @bug
 **/

#ifndef CREEP_H
#define CREEP_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"

struct Point;
struct Vector;
class Timer;
class Sprite;

extern const int CREEP_SIZE;

class EnemyPurple {
	
    public:

    EnemyPurple(Point p, ALLEGRO_COLOR c, Vector s , float *dt);
    ~EnemyPurple();
    
    Point centre;
    ALLEGRO_COLOR color;
    Vector speed;
    bool dead = false;

    void update();
    void load_assets();
    void deathAnim();
    void hit();
    void draw();

    private:
    std::shared_ptr<Timer> fireDelay;   
    std::shared_ptr<Sprite> enemySprite;
    std::shared_ptr<Sprite> deathSprite;
    Vector projSpeed;
    int fireSpeed;
    int lives;
    int dAnim;   
    bool dAnim_complete;
    bool fire;
    float _crtTime = 0;
    float _prevTime = 0;
    float *_dt;
};

#endif
