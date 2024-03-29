#ifndef semaphore_h
#define semaphore_h

#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"
#include "list.h"

__BEGIN_API

class Semaphore
{
public:

    Semaphore(int v = 1) {_size = v; _poll = v;};
    ~Semaphore();

    void p();
    void v();
private:
    // Atomic operations
    int finc(volatile int & number);
    int fdec(volatile int & number);

    // Thread operations
    void sleep();
    void wakeup();
    void wakeup_all();

private:
    //DECLARAÇÃO DOS ATRIBUTOS DO SEMÁFORO
    int _size;
    int _poll;
    std::list<Thread*> _sleeping;

};

__END_API

#endif

