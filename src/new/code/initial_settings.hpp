#ifndef INITIALS_HPP_
#define INITIALS_HPP_

sem_t semaforo;
int index_global;
std::mutex mtx;

int total_arv = 0;
int seed = 0;
int output = 0;
int matrix_t=0;
int num_threads = 1;
int used_threads = 1;
int global_induced_cycle = 1;

bool nolb = false;
bool noindex = false;
bool best = false;
bool abort_for_timeout = false;

float running_time = 0;

pthread_mutex_t mutex_signal;

#endif

