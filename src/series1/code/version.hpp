#pragma once
#include <string>

class Version
{
    private:
        std::string release = "0.9999.0";
        
    public:
        Version();
        ~Version();
        std::string version();
};
