#include "Path.h"
#include <string.h>
#include "Util.h"
#include <assert.h>
#include <sstream>

namespace nfa {

unsigned int Path::m_static_counter = 0;

unsigned int Path::get_counter_id() const
{
    return m_path_counter_id;
}

Path::Path(Path& other)
{
    m_path_counter_id = m_static_counter++;

    m_compartments = 0x00;
    init(other.length());
    for (int i = 0; i < other.length(); ++i)
    {
        m_compartments[i] = other[i];
    }

    m_push_ndx = other.m_push_ndx;
}

Path::Path()
{
    assert(false);
}

Path::Path(unsigned int length)
{
    m_path_counter_id = m_static_counter++;

    m_compartments = 0x00;
    init(length);
    m_push_ndx = 0;
}

void Path:: sort_compartments()
{
    sort_compartments(0, m_length - 1);
}

void Path::sort_compartments(int l, int r) 
{
    int i = l, j = r;
    int tmp;
    int pivot = m_compartments[(l + r) / 2];
    
    while (i <= j) 
    {
        while (m_compartments[i] < pivot)
        {
            i++;
        }
        
        while (m_compartments[j] > pivot)
        {
            j--;
        }
        
        if (i <= j) 
        {
            tmp = m_compartments[i];
            m_compartments[i] = m_compartments[j];
            m_compartments[j] = tmp;
            i++;
            j--;
        }
    }
    
    if (l < j)
    {
        sort_compartments(l, j);
    }
    
    if (i < r)
    {
        sort_compartments(i, r);
    }
}

bool Path::is_full() const
{
    return m_push_ndx == m_length;
}

void Path::pop_back()
{
    --m_push_ndx;
    assert(m_push_ndx >= 0);
}

void Path::push_back(COMPARTMENT_DATA_TYPE compartment_val)
{
    assert(m_push_ndx < m_length);
    m_compartments[m_push_ndx] = compartment_val;
    ++m_push_ndx;
}

std::string Path::to_string()
{
    std::stringstream ss;
    for (int i = 0; i < m_length; ++i)
    {
        ss << m_compartments[i] << " ";
    }

    return ss.str();
}
    
void Path::init(unsigned int length)
{
    if (m_compartments != 0x00)
    {
        delete m_compartments;
    }
    
    m_length = length;
    
    if (m_length > 0)
    {
        m_compartments = new COMPARTMENT_DATA_TYPE[m_length];
    }
    else
    {
        m_compartments = 0x00;
    }
}    

Path::~Path() 
{
    delete[] m_compartments; 
}

unsigned int Path::length() const
{
    return m_length;
}

} //namespace nfa
