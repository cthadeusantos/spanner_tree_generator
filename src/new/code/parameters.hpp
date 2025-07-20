#include <string>

#ifndef PARAMETERS_HPP
    #define PARAMETERS_HPP
extern int num_threads;
extern int global_output;
extern int matrix_t;
extern bool best;
extern bool global_nolb;
extern bool global_noindex;
extern float global_running_time;
extern int global_induced_cycle;
extern bool global_save_tree;
extern int global_closeness;
extern std::string global_path_filetree;
extern bool global_yed;

class Parameters
{
    public:
        static void usage(const char *app_name);
        static void parseArgs(int argc, char** argv);
};

#endif