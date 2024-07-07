#ifndef VERSION_HPP
#define VERSION_HPP
#include <string>

class Version {
private:
    std::string release = "0.99.0";
public:
    Version(const std::string& release = "0.99.0") : release(release) {}  // C++20 default member initialization

    std::string version() const { // Add const for immutability (C++11)
        return release;
    };
};

#endif
