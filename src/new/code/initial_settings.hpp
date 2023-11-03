#ifndef INITIALS_HPP_
#define INITIALS_HPP_

sem_t semaforo;
int index_global;
std::mutex mtx;

int global_total_arv = 0;
int seed = 0;
int global_output = 0;
int matrix_t=0;
int num_threads = 1;
int used_threads = 1;
int global_induced_cycle = 1;
int global_induced_cycle_used = 1;
int global_closeness = 1;

bool global_nolb = false;
bool global_noindex = false;
bool best = false;
bool abort_for_timeout = false;
bool global_save_tree = false;

float global_running_time = 0;

//pthread_mutex_t mutex_signal;

int global_threads_supported = 0;

#endif

