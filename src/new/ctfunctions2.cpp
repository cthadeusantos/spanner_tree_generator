#include <iostream>
#include <sys/stat.h>
//#include <errno.h>
//#include <stdio.h>

#include <string.h>
#include "code/graph.hpp"
#include "code/opBasic.hpp"
#include "code/stretch.hpp"

#include "Debug.h"

#include "ctfunctions2.hpp"

/// Basic debugging controller. See Debug.h for details.
#ifdef MN_BF_SEQ_DEBUG
#define DEBUG
#else
#define DEBUG while (false)
#endif

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

/**
 * @details To swap to values at positions
 * @param *a an integers that represents a value.
 * @param *b an integers that represents a value.
 */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 
/**
 * @details A bubblesort implementation
 * @param array an array with vertex and degree.
 * @param n an integers that represents a value.
 */
void bubbleSort(int **array, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
 
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (array[1][j] < array[1][j + 1]){
                swap(&array[0][j], &array[0][j + 1]);
                swap(&array[1][j], &array[1][j + 1]);
            }
                
}

bool in(std::vector <int> &vector1, const int &value)
{
    bool result = false;
    for(auto vertex : vector1){
        if (vertex == value){
            result = true;
            break;
        }
    }
    return result;
}
