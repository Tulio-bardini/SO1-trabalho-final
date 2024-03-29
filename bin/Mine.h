/**
 * @file Mine.h
 *
 * @author
 * @bug
 **/

#ifndef MINE_H
#define MINE_H

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

class Mine
{

public:
    Mine( float *dt, std::list<Laser> *lasers, bool *finish);
    ~Mine();

    Point centre;
    Vector speed;
    bool dead = false;
    int stage = 0;
    int size;
    int life = 3;
    int row = 0;
    int col = 0;
    std::list<Laser> lasers;

    void update();
    void loadSprite();
    void hit(int damage);
    void draw();
    void explode();

    static void runMine(std::list<std::shared_ptr<Mine>> *mines, bool *finish);

private:
    std::shared_ptr<Timer> _explodeTimer;
    std::shared_ptr<Sprite> mineSprite;
    std::list<Laser> * _lasers;
    bool * _finish;
    float *_dt;
    bool death = false;
};


__END_API

#endif