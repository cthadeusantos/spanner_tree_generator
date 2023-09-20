#include <string>
#include "version.hpp"

//! Constructor
/*!
    Build a graph without vertices or edges
*/
Version::Version()
{
}

//! Destructor
/*!
    Write a detailed description
*/
Version::~Version()
{
   
}

std::string Version::version(){
    return this->release;
}