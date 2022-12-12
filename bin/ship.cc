#include "ship.h"

__BEGIN_API

#define MAX_LIFE 3
#define TypeClassNumber 1

Ship::Ship(bool *finish, std::list<Laser> *lasers, float *dt)
{
   centre = Point(215, 245);
   _finish = finish;
   _dt = dt;
   lasersShip = lasers;

   // Go to resources directory
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));

   shipSprite = std::make_shared<Sprite>("Sprite2.png"); // espaçonave do usuário
}

Ship::~Ship()
{
   shipSprite.reset();
}

void Ship::run()
{

   while (!*_finish && !dead)
   {
      centre = centre + speed * (*_dt);
      selectShipAnimation();
      speed = Vector(0, 0);
      checkBoundary();

      Thread::yield();
   }

   Thread::running()->thread_exit(0);
}

void Ship::draw()
{

   if (!dead)
   {
      shipSprite->draw_region(row, col, 47.0, 40.0, centre, 0);
      drawLife();
   }
}

void Ship::selectShipAnimation()
{
   if (speed.x > 0)
   {
      col = 1;
      if (speed.y > 0)
         row = 2;
      else if (speed.y < 0)
         row = 0;
      else
         row = 1;
   }
   else
   {
      col = 0;
      if (speed.y > 0)
         row = 2;
      else if (speed.y < 0)
         row = 0;
      else
         row = 1;
   }
}

void Ship::checkBoundary()
{
   // check x bound and adjust if out
   if (centre.x > 800 - 16)
      centre.x = 800 - 16;
   else if (centre.x < 16)
      centre.x = 16;
   // check y bound and adjust if out
   if (centre.y > 600 - 16)
      centre.y = 600 - 16;
   else if (centre.y < 16)
      centre.y = 16;
}

void Ship::putX(int x)
{
   speed.x += x;
}

void Ship::putY(int y)
{
   speed.y += y;
}

void Ship::fire()
{
   if (!dead)
   {
      lasersShip->push_back(Laser(centre, al_map_rgb(20, 200, 20), TypeClassNumber, _dt, Vector(500, 0)));
   }
}

void Ship::hit(int damage)
{
   life = life - damage;
   if (life < 1)
   {
      if (lifeRestart > 1)
      {
         life = 3;
         lifeRestart -= 1;
      }
      else
      {
         dead = true;
      }
   }
}

void Ship::drawLife()
{
   if (!dead)
   {
      al_draw_line(centre.x - size * 2, centre.y + size * 2,
                   (centre.x - 2 * size) + (life) * (size * 1.3),
                   centre.y + size * 2,
                   al_map_rgb(255 * (1.0 - life / MAX_LIFE),
                              200 * (life / MAX_LIFE),
                              3),
                   5);

      if (lifeRestart > 0) {
         Point pt_life = Point(30, 30);
         for (int i = 0; i < lifeRestart; i++) {
            al_draw_line(pt_life.x, pt_life.y,
                     pt_life.x + 30,
                   pt_life.y,
                   al_map_rgb(255 * (1.0 - life / MAX_LIFE),
                              200 * (life / MAX_LIFE),
                              0),
                   30);
            pt_life.x = pt_life.x + 45;
         }
      }
   }
}

__END_API