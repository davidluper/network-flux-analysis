#ifndef _PATH_H
#define _PATH_H

#include "NfaIncludes.h"
#include <string.h>
#include <iostream>

using namespace std;

namespace nfa {

class Path
{
friend class PathFactory;
friend class PathFactoryEcoNet;

public:
    Path(Path& other);
    ~Path();

    COMPARTMENT_DATA_TYPE& operator[](unsigned int ndx) const
    {
        return m_compartments[ndx];
    };

    unsigned int get_counter_id() const;
    unsigned int length() const;
    
    void push_back(COMPARTMENT_DATA_TYPE compartment_val);
    std::string to_string();
    bool is_full() const;
    void pop_back();
    void sort_compartments();

    inline bool operator< (const Path& other)
    { 
        for (unsigned int i = 0; i < (*this).length(); ++i)
        {
            if (i >= other.length())
            {
                return false;;
            }
            else if ((*this)[i] < other[i])
            {
                return true;
            }
            else if ((*this)[i] > other[i])
            {
                return false;
            }
        }
        
        return true;
    }

    inline bool operator> (const Path& other)
    {
        for (unsigned int i = 0; i < (*this).length(); ++i)
        {
            if (i >= other.length())
            {
                return true;
            }
            else if ((*this)[i] > other[i])
            {
                return true;
            }
            else if ((*this)[i] < other[i])
            {
                return false;
            }
        }
        
        return false;
    }

    inline bool operator==(const Path& other)
    {
        if ((*this).length() != other.length())
        {
            return false;
        }

        for (unsigned int i = 0; i < (*this).length(); ++i)
        {
            if ((*this)[i] != other[i])
            {
                return false;
            }
        }

        return true;
    }

    inline bool operator!=(const Path& other)
    {
        if ((*this).length() != other.length())
        {
            return true;
        }

        for (unsigned int i = 0; i < (*this).length(); ++i)
        {
            if ((*this)[i] != other[i])
            {
                return true;
            }
        }

        return false;
    }

private:
    Path();
    Path(unsigned int length);
    void init(unsigned int length);

    void sort_compartments(int l, int r);
 
    void load_path(char* s);
    unsigned int get_number_of_compartments_in_path(char* s);

    COMPARTMENT_DATA_TYPE* m_compartments;
    unsigned int m_length;

    unsigned short m_push_ndx;
    unsigned int m_path_counter_id;
    static unsigned int m_static_counter;
};

} //namespace nfa

#endif
