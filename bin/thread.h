#ifndef thread_h
#define thread_h

#include "cpu.h"
#include "traits.h"
#include "debug.h"
#include "list.h"
#include <ctime> 
#include <chrono>
#include <list>

__BEGIN_API

class Thread
{
protected:
    typedef CPU::Context Context;
public:

    typedef Ordered_List<Thread> Ready_Queue;

    // Thread State
    enum State {
        RUNNING,
        READY,
        FINISHING,
        SUSPENDED,
        WAITING
    };

    /*
     * Construtor vazio. Necessário para inicialização, mas sem importância para a execução das Threads.
     */ 
    Thread() { }

    /*
     * Cria uma Thread passando um ponteiro para a função a ser executada
     * e os parâmetros passados para a função, que podem variar.
     * Cria o contexto da Thread.
     * PS: devido ao template, este método deve ser implementado neste mesmo arquivo .h
     */ 
    template<typename ... Tn>
    Thread(void (* entry)(Tn ...), Tn ... an);

    /*
     * Retorna a Thread que está em execução.
     */ 
    static Thread * running() { return _running; }

    /*
     * Retorna o _link da Thread.
     */ 
    Ready_Queue::Element* link() { return &_link; }

    /*
     * Método para trocar o contexto entre duas thread, a anterior (prev)
     * e a próxima (next).
     * Deve encapsular a chamada para a troca de contexto realizada pela class CPU.
     * Valor de retorno é negativo se houve erro, ou zero.
     */ 
    static int switch_context(Thread * prev, Thread * next);

    /*
     * Termina a thread.
     * exit_code é o código de término devolvido pela tarefa (ignorar agora, vai ser usado mais tarde).
     * Quando a thread encerra, o controle deve retornar à main. 
     */  
    void thread_exit (int exit_code);

    /*
     * Retorna o ID da thread.
     */ 
    int id() { return _id; };

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Daspachante (disptacher) de threads. 
     * Executa enquanto houverem threads do usuário.
     * Chama o escalonador para definir a próxima tarefa a ser executada.
     */
    static void dispatcher(); 

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Realiza a inicialização da class Thread.
     * Cria as Threads main e dispatcher.
     */ 
    static void init(void (*main)(void *));


    /*
     * Devolve o processador para a thread dispatcher que irá escolher outra thread pronta
     * para ser executada.
     */
    static void yield(); 

    /*
     * Destrutor de uma thread. Realiza todo os procedimentos para manter a consistência da classe.
     */ 
    ~Thread();

    /*
     * Qualquer outro método que você achar necessário para a solução.
     */ 

    Context* context() { return _context; }

    /*
    * Suspende a thread em execução até que a thread alvo finalize.
    */
    int join();

    /*
     * Coloca a thread que esta executando na fila de threads suspensas.
     */
    void suspend();

    /*
     * Coloca a thread suspensa de volta para a fila de prontos
     */
    void resume();

    /*
     * Coloca a thread de referência na fila de threads sleep
     */
    static void sleep();

    /*
     * Coloca a thread que esta na cabeça da fila de threads sleep na _ready
     */
    static void wakeup(Thread* threadSleeping);

private:
    int _id;
    int _exit_code;
    static int _next_id;
    Context * volatile _context;
    static Thread * _running;

    static Thread _main; 
    static CPU::Context _main_context;
    static Thread _dispatcher;
    static Ready_Queue _ready;
    static std::list<Thread*> _suspended;
    Ready_Queue::Element _link;
    volatile State _state;

    Thread* _joining;

    /*
     * Qualquer outro atributo que você achar necessário para a solução.
     */ 

};

template<typename ... Tn>
inline Thread::Thread(void (* entry)(Tn ...), Tn ... an) /* inicialização de _link */
{
    db<Thread>(TRC) << "Thread::Thread() chamado\n";

    _context = new Context(entry, an...);

    _exit_code = 0;
    _joining = 0;
    _id = _next_id++;
    _state = READY;
    _link = Ready_Queue::Element(this, (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()));
    _ready.insert(&_link);

    db<Thread>(INF) << "Thread created: " << _id << "\n";
}

__END_API

#endif
