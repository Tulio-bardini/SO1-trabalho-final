#include "cpu.h"
#include "debug.h"
#include <iostream>

__BEGIN_API

void CPU::Context::save()
{
    db<CPU>(TRC) << "CPU::Context::save() chamado\n";
    getcontext(&_context);
}

void CPU::Context::load()
{
    db<CPU>(TRC) << "CPU::Context::load() chamado\n";
    setcontext(&_context);
}

CPU::Context::~Context()
{
    db<CPU>(TRC) << "CPU::Context::~Context() chamado\n";
    if (_stack) {
        delete _stack;
    }
}

int CPU::switch_context(Context *from, Context *to)
{
    db<CPU>(TRC) << "CPU::Context::switch_context() chamado\n";
    return swapcontext(&from->_context, &to->_context);
}

int CPU::finc(volatile int & number) {
    db<CPU>(TRC) << "CPU::finc::switch_context() chamado\n";
    int one = 1;
    __asm__ __volatile__ ( "lock ; xadd %0, %1;": "=r"(one) : "m"(number), "0" (one) : "memory");
    return one;
}

int CPU::fdec(volatile int & number) {
    db<CPU>(TRC) << "CPU::fdec::switch_context() chamado\n";
    int one = -1;
    __asm__ __volatile__ ( "lock ; xadd %0, %1;": "=r"(one) : "m"(number), "0" (one) : "memory");
    return one;
}

__END_API
