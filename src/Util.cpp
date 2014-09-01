#include "Util.h"
#include <dirent.h>

namespace nfa {

bool Util::is_numeric(char* ptr)
{
    if (ptr[0] == 0x00)
    {
        return false;
    }

    int cnt = 0;

    if (ptr[0] == '-')
    {
        if (ptr[1] == 0x00)
        {
            return false;
        }
        cnt += 1;
    }

    while (ptr[cnt] != 0x00)
    {
        if (ptr[cnt] < 48 || ptr[cnt] > 57) 
        { 
            return false; 
        }
        
        cnt += 1;
    }

    return true;
}

bool Util::directory_exists(std::string& directory)
{
    bool exists = false;    
    DIR* directory_pointer = opendir(directory.c_str());
    if (directory_pointer != NULL)
    {
        exists = true;    
        (void) closedir(directory_pointer);
    }
    
    return exists;
}

} //namespace nfa
