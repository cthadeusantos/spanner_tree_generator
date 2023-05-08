#ifndef VERSION_HPP
#define VERSION_HPP
#include <string>

class Version
{
    private:
        std::string number = "0.1.7d";
    public:
        Version();
        ~Version();
        std::string version();
};

#endif