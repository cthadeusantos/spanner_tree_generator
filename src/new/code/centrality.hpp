#ifndef CENTRALITY_HPP_
    #define CENTRALITY_HPP_
#endif

#include "graph.hpp"
#include "stretch.hpp"
#include "opBasic.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <bits/stdc++.h>

#include "../Debug.h"

#include "../my_libs/ctfunctions2.hpp"

class Centrality
{
    public:
        Centrality(){}; // motivo de força maior
        ~Centrality(){};

        static int root_selection(Graph &g); // By Thadeu (DEPRECATED)
        static int root_selection2(Graph &g); //By thadeu
        static int root_selection2(std::vector<std::pair<int,float>>);
        static int root_selection2(std::vector<float> vector1);
        static void insertionSort(int arr[], int n); // By Thadeu
        static void my_insertionSort(std::vector <int> &degree, std::vector <float> &closeness, Graph &g ); // By Thadeu
        static void my_insertionSort(std::vector <int> &vector1, std::vector <int> &vector2, char order); // by Thadeu
        static float vertex_importance(int vertex,  Graph &graph); // by Thadeu
        static std::vector <float> closeness_centrality(Graph &graph);
        static std::vector<std::pair<int,float>> closeness_centrality_list(std::set <int> &vertices,Graph &graph);
        static std::vector<std::pair<int,float>> closeness_centrality_list(std::vector <int> &vertices,Graph &graph);
        static std::vector<std::pair<int,float>> closeness_centrality_list(Graph &graph);
        static std::vector<float> closeness_centrality_vector(Graph &graph);
};