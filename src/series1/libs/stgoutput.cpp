#include "stgoutput.hpp"

// /**
//  * Output data function 
//  * @details Show data at screen or file or debug mode
//  * @author Carlos Thadeu
//  */
// void output_data(std::string &run_name, std::string &filename, double &lastExecutionTime, int &lower_limit, Graph &graph){
//     // Define a structure to hold l0abel-value pairs
//     struct Output {
//         std::string label;
//         std::function<void()> printValue;
//     };
    
//     // if (lower_limit == (int)INFINITY)
//     //     lower_limit = 1;
//     if (lower_limit == std::numeric_limits<int>::infinity()) {
//         lower_limit = 1;
//     }

//    // Choose between std::cout and std::cerr
//     //std::ostream *output_stream = &std::cout; // Use a pointer to std::ostream
//     //std::ostream *output_stream = &std::cout; // Use a pointer to std::ostream
//     std::ostream *output_stream = &DEBUG_STREAM; // Use a pointer to std::ostream
//     //output_stream = &DEBUG_STREAM;

//     // if (((global_parameters.global_output & 1) == 1) || ((global_parameters.global_output & 2) == 2)) { // TO SCREEN
//     //     //output_stream = &std::cout;
//     //     output_stream = &DEBUG_STREAM;
//     // } else {
//     //     //output_stream = &DEBUG std::cerr;
//     //     output_stream = &DEBUG_STREAM;
//     // }

//     std::vector<Output> outputs = {
//         {"INSTANCE ............. = ", [&]() { *output_stream << std::setw(10) << filename; }},
//         {"SOLUTION_TYPE ........ = ", [&]() { *output_stream << run_name; }},
//         {"NUM_VERTICES ......... = ", [&]() { *output_stream << graph.numVertices(); }},
//         {"NUM_EDGES ............ = ", [&]() { *output_stream << graph.numEdges(); }},
//         {"LOWER_BOUND .......... = ", [&]() { *output_stream << lower_limit; }},
//         {"STRETCH_INDEX ........ = ", [&]() { *output_stream << graph.stretchIndex(); }},
//         {"TOTAL_TREES .......... = ", [&]() { *output_stream << graph.totalTrees(); }},
//         {"RUNNING_TIME ......... = ", [&]() { *output_stream << lastExecutionTime; }},
//         {"THREADs .............. = ", [&]() { *output_stream << global_parameters.global_num_threads; }},
//         {"MAX_THREADS_SUPPORTED  = ", [&]() { *output_stream << global_parameters.global_max_threads_supported; }},
//         //{"TASKs................. = ", [&]() { *output_stream << used_threads; }},
//         //{"DATE.................. = ", [&]() { *output_stream << current_date(); }},
//         {"APP_RELEASE .......... = ", [&]() { *output_stream << Version().version(); }},
//         //{"CLOSENESS_(HEUR)...... = ", [&]() { *output_stream << get_closeness_type(); }},
//         //{"COMPUTE_INDEX......... = ", [&]() { *output_stream << get_noindex_type(); }},
//         //{"COMPUTE_LOWER_BOUND... = ", [&]() { *output_stream << get_nolb_type(); }},
//         //{"ICYCLES_PROPOSED...... = ", [&]() { *output_stream << global_induced_cycle; }},
//         //{"ICYCLES_SELECTED...... = ", [&]() { *output_stream << global_induced_cycle_used; }}
//     };

//     // Iterate over the vector and print each label-value pair
//     for (const auto& output : outputs) {
//         //std::cout << output.label;
//         *output_stream << output.label;
//         output.printValue();
//         *output_stream << std::endl;
//         //std::cout << std::endl;
//     }
//     if (global_parameters.global_best){
//         //std::cerr << "[BEST TREE]" <<  std::endl;
//         *output_stream << "[BEST_TREE]" << std::endl;
//         int node1 = 0, node2 = 0;
//         for (auto&& tuple: graph.best_tree){
//             std::tie(node1, node2) = tuple;
//             *output_stream << "(" << node1 << " , " << node2 << ") ";
//             //DEBUG(std::cerr << "(" << node1 << " , " << node2 << ") ");
//         }
//     }
//     *output_stream << std::endl << std::endl;
// }

/**
 * Output data function 
 * @details Show data at screen or file or debug mode
 * @author Carlos Thadeu
 */
void output_data(std::string &run_name, std::string &filename, double &lastExecutionTime, Graph &graph){
    // Define a structure to hold l0abel-value pairs
    struct Output {
        std::string label;
        std::function<void()> printValue;
    };
    
    // unsigned int lower_limit;
    // if (lower_limit == std::numeric_limits<int>::infinity()) {
    //     lower_limit = 1;
    // }

   // Choose between std::cout and std::cerr
    std::ostream *output_stream {&DEBUG_STREAM}; // Use a pointer to std::ostream

    std::vector<Output> outputs = {
        {"INSTANCE ............. = ", [&]() { *output_stream << std::setw(10) << filename; }},
        {"SOLUTION_TYPE ........ = ", [&]() { *output_stream << run_name; }},
        {"NUM_VERTICES ......... = ", [&]() { *output_stream << graph.numVertices(); }},
        {"NUM_EDGES ............ = ", [&]() { *output_stream << graph.numEdges(); }},
        {"LOWER_BOUND .......... = ", [&]() { *output_stream << graph.get_lowerBound(); }},
        {"STRETCH_INDEX ........ = ", [&]() { *output_stream << graph.stretchIndex(); }},
        {"TOTAL_TREES .......... = ", [&]() { *output_stream << graph.totalTrees(); }},
        {"RUNNING_TIME ......... = ", [&]() { *output_stream << lastExecutionTime; }},
        {"THREADs .............. = ", [&]() { *output_stream << global_parameters.global_num_threads; }},
        {"MAX_THREADS_SUPPORTED  = ", [&]() { *output_stream << global_parameters.global_max_threads_supported; }},
        //{"TASKs................. = ", [&]() { *output_stream << used_threads; }},
        //{"DATE.................. = ", [&]() { *output_stream << current_date(); }},
        {"APP_RELEASE .......... = ", [&]() { *output_stream << Version().version(); }},
        //{"CLOSENESS_(HEUR)...... = ", [&]() { *output_stream << get_closeness_type(); }},
        //{"COMPUTE_INDEX......... = ", [&]() { *output_stream << get_noindex_type(); }},
        //{"COMPUTE_LOWER_BOUND... = ", [&]() { *output_stream << get_nolb_type(); }},
        //{"ICYCLES_PROPOSED...... = ", [&]() { *output_stream << global_induced_cycle; }},
        //{"ICYCLES_SELECTED...... = ", [&]() { *output_stream << global_induced_cycle_used; }}
    };

    // Iterate over the vector and print each label-value pair
    for (const auto& output : outputs) {
        *output_stream << output.label;
        output.printValue();
        *output_stream << std::endl;
    }
    if (global_parameters.global_best){
        *output_stream << "[BEST_TREE]" << std::endl;
        int node1 = 0, node2 = 0;
        for (auto&& tuple: graph.best_tree){
            std::tie(node1, node2) = tuple;
            *output_stream << "(" << node1 << " , " << node2 << ") ";
        }
    }
    *output_stream << std::endl << std::endl;
}