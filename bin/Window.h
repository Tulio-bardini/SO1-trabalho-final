/**
 * @file Window.h
 * @brief
 *
 * @author
 * @bug
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "ship.h"
#include "controller.h"


__BEGIN_API

// forward declarations
class Menu;
class Root;

class Window {
      
  public:
   Window(int w, int h, int fps);
   ~Window();
   
   void init();
   void run();
   void draw();
   void update(double dt);

   void gameLoop(float& prevTime);

   void drawShip(std::shared_ptr<Sprite> sprite, int flags);
   void drawBackground();

   inline int getWidth() const {
      return _displayWidth;
   }
   inline int getHeight() const {
      return _displayHeight;
   }
   inline int getFps() const {
      return _fps;
   }

   //Static Functions
   static void runShip(Ship * ship);
   static void runController(Controller * controller);

  private:
   void loadSprites();
   //Checks data of the spaceship
   std::shared_ptr<Sprite> spaceShip;
   Point centre;        /**< ship position */
   ALLEGRO_COLOR color; /**< ship color */   
   Vector speed;        /**< movement speed in any direction */
   int row;             /**<row of animation to be played */
   int col;             /**< column of animation to be played */

   //Background
   Point bgMid;/**<point used by the background to draw from */
   Point fgMid;
   Point fg2Mid;
   Vector bgSpeed;/**<background movement speed */
   Vector fgSpeed;
   std::shared_ptr<Sprite> bg;/**<shared pointer to background animation */
   std::shared_ptr<Sprite> fg;

   //ship
   Ship * ship;
   Thread * shipThread;

   //controller
   Controller * controller;
   Thread * controllerThread;


   // general game variables
   int _displayWidth;
   int _displayHeight;
   int _fps;
   // allegro objects
   ALLEGRO_TIMER *_timer;
   ALLEGRO_EVENT_QUEUE *_eventQueue;
   ALLEGRO_DISPLAY *_display;

   bool _finish;
   
};


__END_API
#endif
