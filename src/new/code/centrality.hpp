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
    private:
        static void thread_importance(int start, int end, std::vector<float> &closeness, Graph &graph);
        static void thread_importanceV2(int start, int end, std::vector<float> &closeness, Graph &graph);
        static void thread_leverage(int start, int end, std::vector <float> &leverage, Graph &graph);

    public:
        Centrality(){}; // motivo de força maior
        ~Centrality(){};

        static std::vector<float> leverage(Graph &graph);
        
        static std::vector <float> leverage_centrality_thread(Graph &graph);
        static float vertex_leverage(int root,  Graph &graph);

        static int root_selection(Graph &g); // By Thadeu (DEPRECATED)
        
        static int root_selection2(Graph &g); //By thadeu
        static int root_selection2(std::vector<std::pair<int,float>>);
        static int root_selection2(std::vector<float> vector1);
        
        static int root_selection3(std::vector<float> vertices_closeness, std::vector<float> vertices_leverage);

        static void insertionSort(int arr[], int n); // By Thadeu
        
        static void my_insertionSort(std::vector <int> &vertex, std::vector <float> &closeness, Graph &graph ); // By Thadeu
        static void my_insertionSort(std::vector <int> &vertex, std::vector <float> &closeness, std::vector <float> &leverage, Graph &graph); // By Thadeu
        static void my_insertionSort(std::vector <int> &vector1, std::vector <int> &vector2, char order='a'); // by Thadeu
        static void my_insertionSort(std::vector <float> &vector1, std::vector <int> &vector2, char order='a'); // by Thadeu
        static void my_insertionSort(std::vector<std::pair<int, float>> vector1, char order='a');
        //static void my_insertionSort(std::vector <int> &vector1, std::vector <int> &vector2, std::vector <int> &vector3, char order); // by Thadeu
        
        static float vertex_importance(int vertex,  Graph &graph); // by Thadeu
        static float vertex_importance_v2(int root,  Graph &graph);
        
        static std::vector <float> closeness_centrality(Graph &graph);
        static std::vector <float> closeness_centrality_thread(Graph &graph);
        static std::vector <float> closeness_centrality_thread_V2(Graph &graph);
        
        static std::vector<std::pair<int,float>> closeness_centrality_list(std::set <int> &vertices,Graph &graph);
        static std::vector<std::pair<int,float>> closeness_centrality_list(std::vector <int> &vertices,Graph &graph);
        static std::vector<std::pair<int,float>> closeness_centrality_list(Graph &graph);

        static std::vector<std::pair<int,float>> closeness_centrality_list_V2(std::vector <int> &vertices,Graph &graph);
        
        static std::vector<float> closeness_centrality_vector(Graph &graph);

        static int tiebreaker(std::vector<int> &vertex_list, std::vector<float> &closeness);
        static int tiebreaker(std::vector<int> &vertex_list, std::vector<float> &closeness, std::vector<float> &leverage);
        static int tiebreaker(std::vector<int> &candidates, std::vector<int> &vertex_list, std::vector<float> &closeness, std::vector<float> &leverage);

        static std::vector<std::pair<int,float>> leverage_centrality_list(Graph &graph);
        static std::vector<std::pair<int,float>> leverage_centrality_list(std::vector <float> &leverage);
};