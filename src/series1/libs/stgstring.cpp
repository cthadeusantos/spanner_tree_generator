#include "stgstring.hpp"

/**
 * Split a string
 * @details Split a string
 * The delimiter is not optional because there isn't implemented default, you must select one.
 * 
 * Reference:
 * https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
 * Solution 1.3 (Adapted to spanner tree generator)
 * @author Carlos Thadeu
 * @param s a string
 * @param delimiter a char that represents a delimiter of a string
 * @return a string that represents the name of file
 */
std::vector<std::string> split(const std::string& s, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)) {
      tokens.push_back(token);
   }
   return tokens;
}