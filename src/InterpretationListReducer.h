#ifndef INTERPRETATION_LIST_REDUCER_H
#define INTERPRETATION_LIST_REDUCER_H

#include "collections/LinkedList.h"
#include "Path.h"

namespace nfa {

class InterpretationListReducer
{

#define INTERPRETATION_LIST_REDUCER_HASH_TABLE_LENGTH 1024
public:
    InterpretationListReducer(LinkedList<Path*>& flux_labeled_paths);
    ~InterpretationListReducer();    
    void reduce(LinkedList<Path*>& reduced_paths);

private:
    unsigned short compute_hash_key_for_path(Path& path);
    bool path_already_exists(Path* path);
  
    LinkedList<Path*>& m_flux_labeled_paths;
    LinkedList<Path*>* m_hash_store[INTERPRETATION_LIST_REDUCER_HASH_TABLE_LENGTH];
};

} //namespace nfa

#endif
