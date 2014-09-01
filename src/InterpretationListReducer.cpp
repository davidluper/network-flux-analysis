#include "InterpretationListReducer.h"

namespace nfa {

InterpretationListReducer::InterpretationListReducer(LinkedList<Path*>& flux_labeled_paths) :
    m_flux_labeled_paths(flux_labeled_paths)
{
    for (int i = 0; i < INTERPRETATION_LIST_REDUCER_HASH_TABLE_LENGTH; ++i)
    {
        m_hash_store[i] = 0x00;
    }
}

InterpretationListReducer::~InterpretationListReducer()
{
    for (int i = 0; i < INTERPRETATION_LIST_REDUCER_HASH_TABLE_LENGTH; ++i)
    {
        delete m_hash_store[i];
    }
}

unsigned short InterpretationListReducer::compute_hash_key_for_path(Path& path)
{
    unsigned long hash_key = 0;
    unsigned short repeat_count = 1;
    for (unsigned int i = 0; i < path.length(); ++i)
    {
        if (i > 0)
        {
            if (path[i] == path[i - 1])
            {
                ++repeat_count;
            }
            else
            {
                repeat_count = 1;
            }
        }

        hash_key += (i * path[i] * repeat_count * 31);
    }

    return hash_key % INTERPRETATION_LIST_REDUCER_HASH_TABLE_LENGTH;
}

bool InterpretationListReducer::path_already_exists(Path* path)
{
    unsigned short hash_key = compute_hash_key_for_path(*path);
    if (m_hash_store[hash_key] == 0x00)
    {
        m_hash_store[hash_key] = new LinkedList<Path*>();
        m_hash_store[hash_key]->push_back(path);
        return false;
    }
    else
    {
        for (LinkedList<Path*>::iterator it = m_hash_store[hash_key]->begin();
             it != m_hash_store[hash_key]->end(); 
             ++it)
        {
            if (*path == (*(*it)))
            {
                return true;
            }
        }

        m_hash_store[hash_key]->push_back(path);
        return false;
    }
}

void InterpretationListReducer::reduce(LinkedList<Path*>& reduced_paths)
{
    unsigned short hash_key;
    for (nfa::LinkedList<nfa::Path*>::iterator it = m_flux_labeled_paths.begin(); it != m_flux_labeled_paths.end(); ++it)
    {
        (*it)->sort_compartments();
        //check if the path has already been place in the reduced_paths list
        //if it has not mark that it has, this check inserts into the hash table
        //if the hash table does not contain the path
        if (!path_already_exists((*it)))
        {
            reduced_paths.push_back((*it));
        }
    }
}

} //namespace nfa
