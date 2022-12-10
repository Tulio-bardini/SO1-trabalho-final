/**
 * @file Laser.cc
 * @brief Implementation of Laser class- draws, updates and destroys laser object
 *
 * @author
 */
#include "Laser.h"

#include <allegro5/allegro_primitives.h>

Laser::Laser(Point p, ALLEGRO_COLOR c, Vector s)
{
   centre = p;
   speed = s;
   color = c;
   live = true;
}

Laser::~Laser() {
   
}

void Laser::update() {
   centre = centre + speed * 0.02;
   if (!in_bound())
      live = false;
}

void Laser::draw() {
   Point tracer = (centre) + (speed) * (-0.04);
   al_draw_line(centre.x, centre.y, tracer.x, tracer.y, color, 3);
}

bool Laser::in_bound() {
   if ((centre.x > 800) ||
       (centre.x < 0) ||
       (centre.y > 600) ||
       (centre.y < 0))
      return false;
   return true;
}