#include <ucontext.h>
#include <iostream>
#include <cstdint>

using namespace std;

typedef void (*thread_startfunc_t) (void *);
static const unsigned int STACK_SIZE = 262144;

ucontext_t* init_context(thread_startfunc_t func, void*);

int global = 0;

ucontext_t *main_c, *c1, *c2;

void print_args(intptr_t arg) {
  cout << "enter print_args" << endl;
  cout << "print_arg: " << arg << endl;

  arg += global;
  global += 1;
  cout << "arg after incr by global: " << arg << endl;

  cout << "finish print_args" << endl;
  if (arg == 1) {
    setcontext(main_c);
  }
}

int main() {
  main_c = init_context(nullptr, nullptr);
  c1 = init_context((thread_startfunc_t) print_args, (void*) 1);
  c2 = init_context((thread_startfunc_t) print_args, (void*) 2);
  swapcontext(main_c, c1);
  swapcontext(main_c, c2);
  cout << "done" << endl;
}

ucontext_t* init_context(thread_startfunc_t func, void* arg) {
  ucontext_t* ucp = new ucontext_t;
  getcontext(ucp);  // initialize ucontext, as required by interface
  ucp->uc_link = nullptr;
  if (func) {
    char* stack = new char[STACK_SIZE];
    ucp->uc_stack.ss_sp = stack;
    ucp->uc_stack.ss_size = STACK_SIZE;
    ucp->uc_stack.ss_flags = 0;
    makecontext(ucp, (void (*)()) func, 1, arg);
  }
  return ucp;
}
