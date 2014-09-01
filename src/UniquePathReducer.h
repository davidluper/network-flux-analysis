#ifndef UNIQUE_PATH_REDUCER_H
#define UNIQUE_PATH_REDUCER_H

#include "Path.h"
#include "collections/LinkedList.h"
#include "RuntimeContext.h"
#include <string>

namespace nfa {

/*
 * this class is responsible for reducing an input path file down to its
 * set of unique paths.  no sense reinterpreting a path when we can just apply
 * a multiplier
 */
class UniquePathReducer {

#define UNIQUE_PATH_REDUCER_HASH_TABLE_LENGTH 16384

struct Node {
public:
    Node(Path* path, unsigned int count) {
        m_path = path;
        m_count = count;
    }
    
    ~Node() {

    }
    
    Path* m_path;
    unsigned int m_count;
};

public:
    UniquePathReducer(RuntimeContext& context);
    void make_unique_file();
    ~UniquePathReducer();
   
private:
    void write_unique_path_file();
    void add_path(Path* path);
    unsigned int compute_hash_key(Path& path);
    
    RuntimeContext& m_context;
    LinkedList<Node*>* m_hash_store[UNIQUE_PATH_REDUCER_HASH_TABLE_LENGTH];
};

} //namespace nfa

#endif

