#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace nfa {

class Util
{
 public:
    static bool is_numeric(char* ptr);
    static bool directory_exists(std::string& directory);

};

}

#endif
