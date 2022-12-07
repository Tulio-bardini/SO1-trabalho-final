#include "thread.h"
#include "cpu.h"
#include "main_class.h"
#include "debug.h"
#include <iostream>
#include <limits.h>
#include "list.h"

__BEGIN_API

int Thread::_next_id = 0;
Ordered_List<Thread> Thread::_ready = Ordered_List<Thread>();
std::list<Thread*> Thread::_suspended = std::list<Thread*>();
Thread *Thread::_running = 0;
Thread Thread::_main = Thread(); 
Thread Thread::_dispatcher = Thread();
CPU::Context Thread::_main_context = CPU::Context();

int Thread::switch_context(Thread * prev, Thread * next)
{
    db<Thread>(TRC) << "Thread::switch_context() chamado\n";
    db<Thread>(INF) << "Thread switch: " << prev->_id << " -> " << next->_id << "\n";
    _running = next;
    return CPU::switch_context(prev->_context, next->_context);
}

void Thread::thread_exit (int exit_code)
{
    db<Thread>(TRC) << "Thread::thread_exit() chamado\n";
    db<Thread>(INF) << "Thread exited: " << _id << "\n";
    _state = FINISHING;
    _exit_code = exit_code;
    _next_id -= 1;

    if (_joining != 0) {
        _joining->resume();
    }

    yield();
}

void Thread::init(void (*main)(void *)) {
    db<Thread>(TRC) << "Thread::thread_init() chamado\n";

    new (&_main) Thread(main,(void*) "main");

    new (&_dispatcher) Thread(Thread::dispatcher);
    
    _dispatcher._link.rank(INT_MIN);

    db<Thread>(INF) << "Main Thread ID: " << Thread::_main.id() << "\n";
    db<Thread>(INF) << "Dispatcher Thread ID: " << Thread::_dispatcher.id() << "\n";

    new (&_main_context) CPU::Context();
    _running = &_main;
    _running->_state = RUNNING;
    _ready.remove(&_main);

    CPU::switch_context(&_main_context, _main.context());
}

void Thread::dispatcher() {
    db<Thread>(TRC) << "Thread::dispatcher() chamado\n";

    while (_ready.size() > 0) {        
        Thread *next_thread = _ready.remove_head()->object();

        _dispatcher._state = READY;
        _ready.insert(&_dispatcher._link);
        
        _running = next_thread;
        _running->_state = RUNNING;

        switch_context(&_dispatcher, _running);

        if (next_thread->_state == FINISHING) {
            _ready.remove(next_thread);
        }
    }

    for (auto const &thread : _suspended)
    {
        db<Thread>(WRN) << "Thread " << thread->id() << " was suspended but never resumed. Deleting thread.";
        if (thread != &_main)
        {
            delete thread;
        }
    }

}

void Thread::yield()
{

    db<Thread>(TRC) << "Thread::yield chamado\n";
    db<Thread>(INF) << "Yielding Thread: " << _running->_id << "\n";
    db<Thread>(INF) << "_ready size: " << _ready.size() << "\n";

    if (_running != &_dispatcher &&
        _running != &_main)
    {
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        _running->_link.rank(now);
    }

    if (_running->_state == RUNNING)
    {
        _running->_state = READY;
    }

    if (_running->_state != SUSPENDED && _running->_state != WAITING)
    {
        _ready.insert(&_running->_link);
    }

    Thread* current_thread = _running;
    _running = _ready.remove_head()->object();
    _running->_state = RUNNING;

    Thread::switch_context(current_thread, &_dispatcher);
}

void Thread::suspend() {
    db<Thread>(TRC) << "Thread::suspend chamado\n";
    db<Thread>(INF) << "SUSPEND: Suspended thread " << _id << "\n";

    if (_state == SUSPENDED || this == &_dispatcher) {
        return;
    }

    _state = SUSPENDED;
    _ready.remove(this);
    _suspended.push_back(this);

    if (this == _running) {
        yield();
    }

}

void Thread::resume() {
    db<Thread>(TRC) << "Thread::resume chamado\n";
    db<Thread>(INF) << "RESUME: Resumed thread " << _id << "\n";

    if (_state != SUSPENDED) {
        return;
    }

    _state = READY;
    _suspended.remove(this);

    _ready.insert(&_link);
}

int Thread::join() {
    db<Thread>(TRC) << "Thread::join chamado\n";
    db<Thread>(INF) << "JOIN: Thread " << _running->_id << " is joining Thread " << _id << "\n";

    if (_state == FINISHING) {
        return _exit_code;
    }

    _joining = _running;
    _running->suspend();

    return _exit_code;
}

void Thread::sleep() {
    db<Thread>(TRC) << "Thread::sleep chamado\n";
    if (_running == &_dispatcher) {
        return;
    }
    db<Thread>(INF) << "SLEEP: Thread " << _running->id() << "\n";
    _running->_state = WAITING;

    yield();

}

void Thread::wakeup(Thread* threadSleeping) {
    db<Thread>(TRC) << "Thread::wakeup chamado\n";
    db<Thread>(INF) << "WAKEUP: Thread " << threadSleeping->id() << "\n";

    threadSleeping->_state = READY;
    _ready.insert(&threadSleeping->_link);

    yield();

}

Thread::~Thread() {
    db<Thread>(TRC) << "Thread::~Thread() chamado\n";
    db<Thread>(INF) << "Thread " << _id << " destruída\n";
    delete _context;
}

__END_API
