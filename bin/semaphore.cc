#include "cpu.h"
#include "thread.h"
#include "traits.h"
#include "debug.h"
#include "semaphore.h"
#include "cpu.h"

__BEGIN_API

void Semaphore::p() {
    db<Semaphore>(TRC) << "Semaphore::p chamado\n";
    db<Semaphore>(INF) << "poll semaphoro = " << _poll << " p()";
    if (fdec(_poll) <= 0) {
        sleep();
    }

}

void Semaphore::v() {
    db<Semaphore>(TRC) << "Semaphore::v chamado\n";
    db<Semaphore>(INF) << "poll semaphoro = " << _poll << " v()" ;
    // Quando todas as threads ja tiverem sido liberadas
    if (_poll >= _size) {
        db<Semaphore>(WRN) << "Todas as Threads foram liberadas\n";
        return;
    }

    if (finc(_poll) < 0) {
        wakeup();
    }

}

int Semaphore::finc(volatile int & number) {
    db<Semaphore>(TRC) << "Semaphore::finc chamado\n";
    return CPU::finc(number);
}

int Semaphore::fdec(volatile int & number) {
    db<Semaphore>(TRC) << "Semaphore::fdec chamado\n";
    return CPU::fdec(number);
}

void Semaphore::sleep() {
    db<Semaphore>(TRC) << "Semaphore::sleep chamado\n";
    Thread* running = Thread::running();
    _sleeping.push_back(running);
    running->sleep();
}

void Semaphore::wakeup() {
    db<Semaphore>(TRC) << "Semaphore::wakeup chamado\n";
    if (_sleeping.size() != 0) {
        Thread* threadSleeping = _sleeping.front();
        _sleeping.pop_front();

        Thread::wakeup(threadSleeping);
    }

}

void Semaphore::wakeup_all() {
    db<Semaphore>(TRC) << "Semaphore::wakeup_all chamado\n";
    if (_sleeping.size() != 0) {
        for (int i = _sleeping.size(); i > 0; i--) {
            wakeup();
        }
    }

}

Semaphore::~Semaphore() {
    db<Semaphore>(TRC) << "Semaphore::~Semaphore chamado\n";
    wakeup_all();
}

__END_API