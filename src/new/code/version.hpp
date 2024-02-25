#ifndef VERSION_HPP
#define VERSION_HPP
#include <string>

class Version
{
    private:
        std::string release = "0.2.11";
    public:
        Version();
        ~Version();
        std::string version();
};

#endif