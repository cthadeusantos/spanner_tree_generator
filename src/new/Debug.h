/**
 * @file Debug.h
 * @author Anderson Zudio (azudio@id.uff.br)
 * @brief This file controls what components should be debugged. 
 * @details To use this, you should compile everything in DEBUG_MODE (see makefile for details). Individually uncomment lines inside this file to debug the respective component.
 * Each component has a basic debug controller. When activated, debug code will run to help the analysis of the component with prints in std::cerr.
 */
#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG

///Debug main_BF-SEQ.cpp
#define MN_BF_SEQ_DEBUG 1 

#endif //NDEBUG

#endif //DEBUG_H