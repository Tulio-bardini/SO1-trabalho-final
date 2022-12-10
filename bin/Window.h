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

   void addLaser(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd);
   void spawEnemys();

   void gameLoop(float& prevTime);

   void drawBackground();
   void drawLaser();
   void drawEnemys();

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
   static void runLaser(std::list<Laser> *laserVector,  bool * finish);
   static void runEnemys(std::list< std::shared_ptr<EnemyPurple> > *enemyList, bool * finish);

  private:
   void loadSprites();
   //Checks data sprites
   std::list<Laser> laserList;
   std::list< std::shared_ptr<EnemyPurple> > enemyList; 
   std::shared_ptr<Timer> _WeaponTimer;
   std::shared_ptr<Timer> _EnemyTimer;
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

   //laser
   Thread * laserThread;

   //enemys
   Thread * enemyThread;

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
