#ifndef main_class_h
#define main_class_h

#include <iostream>
#include <allegro5/allegro.h>
#include "cpu.h"
#include "traits.h"
#include "thread.h"
#include "semaphore.h"
#include "Window.h"
#include <ctime>
#include <cstdlib>
#include <string>

__BEGIN_API

class Main
{
public:
    Main()
    {
    }

    static void run(void *name)
    {
        std::cout << (char *)name << ": inicio\n";

        Window shooty(800, 600, 60);

        game = new Thread(runGame, &shooty);

        int ec = game->join();
        game->thread_exit(ec);

        std::cout << (char *)name << ": fim\n";

        delete game;
    }

    ~Main() {}

private:

    static void runGame(Window *game)
    {
        srand(time(0));
        game->init();
        game->run();
        Thread::running()->thread_exit(0);
    }

private:
    static Thread *game;
};

__END_API

#endif
