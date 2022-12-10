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

class Laser {
  public:
    Laser (Point p, ALLEGRO_COLOR c, Vector s);   
    ~Laser();

    Point centre;
    bool live;
    ALLEGRO_COLOR color;

    void update();
    void draw();
    Vector speed;

    private:
    bool in_bound();
    
};

#endif