#pragma once
#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <optional>
#include <variant>
#include <string>
#include <filesystem>
#include <thread>  // std::thread
#include <iostream>

#include "version.hpp"

class Parameters {
public:
    unsigned short int global_max_threads_supported;
    unsigned short int global_num_threads;
    unsigned short int global_used_threads;
    int global_output;
    int global_matrix_t;
    int global_induced_cycle;
    int global_induced_cycle_used;
    int global_closeness;
    int global_seed;

    unsigned long long int global_total_arv; // Probably deprecated

    bool global_best;
    bool global_nolb;
    bool global_noindex;
    bool global_save_tree;
    bool global_abort_for_timeout;
    
    float global_running_time;

    void usage(const std::string &app_name) const;
    void parseArgs(int argc, char** argv);

    // int get_threads_supported();

    // Constructor to initialize the parameters with default values
    Parameters();

private:
    // Private helper function to show the usage
    //void showUsage() const;
};

static Parameters global_parameters;

#endif
