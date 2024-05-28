#include <string>
#include "version.hpp"

//! Constructor
Version::Version()
{
}

//! Destructor
Version::~Version()
{
   
}

std::string Version::version(){
    return this->release;
}