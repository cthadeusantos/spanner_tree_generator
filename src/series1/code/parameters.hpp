#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <optional>
#include <variant>
#include <string>
#include <filesystem>

class Parameters {
public:
    std::optional<int> global_num_threads;
    std::optional<int> global_output;
    std::optional<int> global_matrix_t;
    std::optional<bool> global_best;
    std::optional<bool> global_nolb;
    std::optional<bool> global_noindex;
    std::optional<float> global_running_time;
    std::optional<int> global_induced_cycle;
    std::optional<bool> global_save_tree;
    std::optional<int> global_closeness;

    void usage(const std::string &app_name) const;
    void parseArgs(int argc, char** argv);

    // Constructor to initialize the parameters with default values
    Parameters();

private:
    // Private helper function to show the usage
    //void showUsage() const;
};

#endif
