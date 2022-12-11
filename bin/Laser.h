/**
 * @file Laser.h
 * @brief derived class of Projectile
 *
 * @author
 * @bug
 */

#ifndef LASER_H
#define LASER_H

#include <allegro5/allegro.h>

#include "Point.h"
#include "Vector.h"
#include "Sprite.h"
#include "Action.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Laser.h"

__BEGIN_API

class Laser {
  public:
    Laser(Point p, ALLEGRO_COLOR c, Vector s);   
    ~Laser();

    Point centre;
    bool live;
    ALLEGRO_COLOR color;

    static void runLaser(std::list<Laser> *laserVector, bool *finish);
    void update();
    void draw();
    Vector speed;

    private:
    bool in_bound();
    
    
};

__END_API

#endif