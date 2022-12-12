/**
 * @file Window.h
 * @brief
 *
 * @author
 * @bug
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"
#include "Vector.h"
#include "Action.h"
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "ship.h"
#include "controller.h"
#include "Laser.h"
#include "Timer.h"
#include "enemyPurple.h"
#include "colider.h"
#include "Mine.h"

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
   void update();
   void setTimers();

   void fire();
   void spawEnemies();
   void spawMine();

   void gameLoop(float& prevTime);

   void drawBackground();
   void drawEnemies();
   void drawMine();
   void drawLasers();

   // Threads
   void createThreads();
   void deleteThreads();

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
   static void runColider(Colider *colider);

   std::list<Laser> lasers;

  private:
   void loadSprites();
   //Checks data sprites
   std::list< std::shared_ptr<EnemyPurple> > enemyList;
   std::list< std::shared_ptr<Mine> > mines;
   std::shared_ptr<Timer> _WeaponTimer;
   std::shared_ptr<Timer> _EnemyTimer;
   std::shared_ptr<Timer> _MineTimer;

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

   //colider
   Colider * colider;
   Thread * coliderThread;

   //mine
   Thread * mineThread;

   //enemys
   Thread * enemyThread;

   // Enemies Lasers
   Thread * enemyLasersThread;

   // general game variables
   int _displayWidth;
   int _displayHeight;
   int _fps;
   float dt;
   float crtTime = 0;
   float prevTime = 0;
   // allegro objects
   ALLEGRO_TIMER *_timer;
   ALLEGRO_EVENT_QUEUE *_eventQueue;
   ALLEGRO_DISPLAY *_display;

   bool _finish;
   
};

__END_API

#endif
