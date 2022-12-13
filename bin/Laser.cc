/**
 * @file Laser.cc
 * @brief Implementation of Laser class- draws, updates and destroys laser object
 *
 * @author
 */
#include "Laser.h"

__BEGIN_API

Laser::Laser(Point p, ALLEGRO_COLOR c, int owner, float *dt, Vector s)
{
   centre = p;
   speed = s;
   color = c;
   live = true;
   _dt = dt;
   classOwner = owner;
}

Laser::~Laser()
{
}

void Laser::update()
{
   centre = centre + speed * (*_dt);
   if (!in_bound())
      live = false;
}

void Laser::draw()
{
   Point tracer = (centre) + (speed) * (-0.04);
   al_draw_line(centre.x, centre.y, tracer.x, tracer.y, color, 3);
}

bool Laser::in_bound()
{
   if ((centre.x > 1000) ||
       (centre.x < 0) ||
       (centre.y > 600) ||
       (centre.y < 0))
      return false;
   return true;
}

void Laser::runLaser(std::list<Laser> *laserVector, bool *finish)
{

   while (!*finish)
   {
      if (!laserVector->empty())
      {
         for (std::list<Laser>::iterator it = laserVector->begin();
              it != laserVector->end(); ++it)
         {
            it->update();
         }
         std::list<Laser> newLaserVector;
         for (auto it = laserVector->begin(); it != laserVector->end(); ++it)
         {
            if (it->live)
            {
               newLaserVector.push_back(*it);
            }
         }
         laserVector->clear();
         laserVector->assign(newLaserVector.begin(), newLaserVector.end());
      }
      Thread::yield();
   }

   Thread::running()->thread_exit(0);
}

__END_API