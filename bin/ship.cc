#include "ship.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>

#include "Timer.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"

__BEGIN_API

Ship::Ship(bool *finish)
{
   centre = Point(215, 245);
   _finish = finish;

    // Go to resources directory
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));

   shipSprite = std::make_shared<Sprite> ("Sprite2.png"); //espaçonave do usuário
}

Ship::~Ship() {
   shipSprite.reset();
}

void Ship::run() {
   
   while (!*_finish)
   {
      _crtTime = al_current_time();
      centre = centre + speed * (_crtTime - _prevTime);
      selectShipAnimation();
      _prevTime = _crtTime;
      speed = Vector(0, 0);
      checkBoundary();

      Thread::yield();
   }
   Thread::running()->thread_exit(0);
    
}

void Ship::draw() {

   shipSprite->draw_region(row, col, 47.0, 40.0, centre, 0);

}

void Ship::selectShipAnimation() {
   if (speed.x > 0) {
      col = 1;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   } else {
      col = 0;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   }
}

void Ship::checkBoundary() {   
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

void Ship::putX(int x) {
    speed.x += x;
}

void Ship::putY(int y) {
    speed.y += y;
}


__END_API