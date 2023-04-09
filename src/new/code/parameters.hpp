#ifndef PARAMETERS_HPP
    #define PARAMETERS_HPP
extern int num_threads;
extern int output;
extern int matrix_t;
extern bool best;
extern bool nolb;
extern float running_time;

class Parameters
{
    public:
        static void usage(const char* app_name);
        static void parseArgs(int argc, char** argv);
};

#endif