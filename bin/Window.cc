#include "Window.h"

__BEGIN_API

#define WEAPON_DELAY_LASER_VERSUS 10
#define ORDER_ENEMYS_DELAY 200
#define MINE_DELAY 160

Window::Window(int w, int h, int fps) : _displayWidth(w), _displayHeight(h),
                                        _fps(fps),
                                        _timer(NULL),
                                        _eventQueue(NULL),
                                        _finish(false)
{
}

Window::~Window()
{
   if (_timer != NULL)
      al_destroy_timer(_timer);
   if (_eventQueue != NULL)
      al_destroy_event_queue(_eventQueue);
   if (_display != NULL)
      al_destroy_display(_display);

   bg.reset();
}

// initialize Allegro, the _display window, the addons, the timers, and event
// sources
void Window::init()
{
   // initialize allegro
   al_init();
   // create the display
   if ((_display = al_create_display(_displayWidth, _displayHeight)) == NULL)
   {
      std::cout << "Cannot initialize the display\n";
      exit(1);
   }

   // initialize addons
   al_init_primitives_addon();
   al_init_font_addon();
   al_init_ttf_addon();
   al_init_image_addon();
   // initialize our timers
   if ((_timer = al_create_timer(1.0 / _fps)) == NULL)
   {
      std::cout << "error, could not create timer\n";
      exit(1);
   }
   if ((_eventQueue = al_create_event_queue()) == NULL)
   {
      std::cout << "error, could not create event queue\n";
      exit(1);
   }
   // register our allegro _eventQueue
   al_register_event_source(_eventQueue, al_get_display_event_source(_display));
   al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
   al_start_timer(_timer);
   // install keyboard
   if (!al_install_keyboard())
   {
      std::cerr << "Could not install keyboard\n";
   }

   crtTime = al_current_time();
   dt = crtTime - prevTime;
   prevTime = crtTime;

   setTimers();

   // register keyboard
   al_register_event_source(_eventQueue, al_get_keyboard_event_source());

   createThreads();
   loadSprites();
}

// repeatedly call the state manager function until the _state is EXIT
void Window::run()
{
   float prevTime = 0;
   // main engine loop
   while (!_finish)
   {
      gameLoop(prevTime);
   }

   deleteThreads();
}

void Window::gameLoop(float &prevTime)
{
   ALLEGRO_EVENT event;
   // ALLEGRO_KEYBOARD_STATE kb;
   bool redraw = true;
   float crtTime;

   // input
   al_get_keyboard_state(&controller->kb);

   // get event
   al_wait_for_event(_eventQueue, &event);

   // _display closes
   if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || controller->acao == act::action::QUIT_GAME)
   {
      _finish = true;
      return;
   }

   if (controller->acao == act::action::FIRE_SECONDARY)
   {
      fire();
      controller->acao = act::action::NO_ACTION;
   }

   crtTime = al_current_time();
   dt = crtTime - prevTime;
   prevTime = crtTime;

   spawEnemys();
   spawMine();

   // timer
   if (event.type == ALLEGRO_EVENT_TIMER)
   {
      crtTime = al_current_time();
      update();
      prevTime = crtTime;
      redraw = true;
   }

   // render
   if (redraw && al_is_event_queue_empty(_eventQueue))
   {
      redraw = false;
      draw();
      al_flip_display();
   }

   Thread::yield();
}

// update the game mode
void Window::update()
{
   // background
   bgMid = bgMid + bgSpeed * dt;
   if (bgMid.x >= 800)
   {
      bgMid.x = 0;
   }
}

// draws for the game mode
void Window::draw()
{
   drawBackground();
   drawEnemies();
   drawLasers();
   drawMine();
   ship->draw();
   ship->drawLaser();
}

void Window::drawBackground()
{
   bg->draw_parallax_background(bgMid.x, 0);
}

void Window::drawEnemies()
{
   if (!enemyList.empty())
   {
      for (auto it = enemyList.begin(); it != enemyList.end(); ++it)
      {
         (*it)->draw();
      }
   }
}

void Window::drawMine()
{
   if (!mines.empty())
   {
      for (auto it = mines.begin(); it != mines.end(); ++it)
      {
         (*it)->draw();
      }
   }
}

void Window::drawLasers()
{
   if (!enemyLasers.empty())
    {
        for (std::list<Laser>::iterator it = enemyLasers.begin();
             it != enemyLasers.end(); ++it)
        {
            it->draw();
        }
    }
}

void Window::loadSprites()
{
   // represents the middle of the image width-wise, and top height-wise
   bgMid = Point(0, 0);
   fgMid = Point(800, 0);
   fg2Mid = Point(300, 300);
   bgSpeed = Vector(50, 0);
   fgSpeed = Vector(-90, 0);

   // Go to resources directory
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));
   // sprites

   bg = std::make_shared<Sprite>("BGstars.png"); // fundo da tela - background
   // delete path
   al_destroy_path(path);
}

void Window::spawEnemys()
{

   if (_EnemyTimer->getCount() > ORDER_ENEMYS_DELAY)
   {

      int cases = rand() % 1;

      Vector enemySpeed(-180, 0);

      switch (cases)
      {
      case 0: // V wave
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1200, 300),
            enemySpeed, &enemyLasers, &dt, &_finish));
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1300, 350),
            enemySpeed, &enemyLasers, &dt, &_finish));
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1300, 250),
            enemySpeed, &enemyLasers, &dt, &_finish));
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1400, 400),
            enemySpeed, &enemyLasers, &dt, &_finish));
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1400, 200),
            enemySpeed, &enemyLasers, &dt, &_finish));
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1500, 100),
            enemySpeed, &enemyLasers, &dt, &_finish));
         enemyList.push_back(std::make_shared<EnemyPurple>(Point(1500, 500),
            enemySpeed, &enemyLasers, &dt, &_finish));

         break;

      default:
         break;
      }

      _EnemyTimer->srsTimer();
   }
}

void Window::spawMine()
{
   if (_MineTimer->getCount() > MINE_DELAY) {
      mines.push_back(std::make_shared<Mine> (&dt, &enemyLasers, &_finish));
      _MineTimer->srsTimer();
   }

}

void Window::fire()
{
   if (_WeaponTimer->getCount() > WEAPON_DELAY_LASER_VERSUS)
   {
      ship->fire();
      _WeaponTimer->srsTimer();
   }
}

void Window::createThreads()
{
   // Create Ship
   ship = new Ship(&_finish, &dt);
   shipThread = new Thread(runShip, ship);

   // Create Controller
   controller = new Controller(&_finish, ship);
   controllerThread = new Thread(runController, controller);

   colider = new Colider(ship, &enemyList, &enemyLasers, &mines, &_finish);
   coliderThread = new Thread(runColider, colider);

   // Create Enemys
   enemyThread = new Thread(EnemyPurple::runEnemies, &enemyList, &_finish);

   // Laser Thread
   enemyLasersThread = new Thread(Laser::runLaser, &enemyLasers, &_finish);

   // Mines Thread
   mineThread = new Thread(Mine::runMine, &mines, &_finish);
}

void Window::deleteThreads()
{
   shipThread->join();
   controllerThread->join();
   enemyThread->join();
   coliderThread->join();
   enemyLasersThread->join();

   delete shipThread;
   delete controller;
   delete enemyThread;
   delete coliderThread;
   delete enemyLasersThread;
}

void Window::setTimers() {
   // Start timers
   _WeaponTimer = std::make_shared<Timer>(_fps);
   _WeaponTimer->create();
   _WeaponTimer->startTimer();
   _EnemyTimer = std::make_shared<Timer>(_fps);
   _EnemyTimer->create();
   _EnemyTimer->startTimer();
   _MineTimer = std::make_shared<Timer>(_fps);
   _MineTimer->create();
   _MineTimer->startTimer();
}

void Window::runShip(Ship *ship)
{
   ship->run();
   delete ship;
}

void Window::runController(Controller *controller)
{
   controller->input();
}

void Window::runColider(Colider *colider)
{
   colider->run();
}


__END_API
