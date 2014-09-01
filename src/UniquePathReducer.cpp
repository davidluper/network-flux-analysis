#include "UniquePathReducer.h"
#include "PathFactory.h"
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

namespace nfa {

UniquePathReducer::UniquePathReducer(RuntimeContext& context) :
    m_context(context) {
    for (unsigned int i = 0; i < UNIQUE_PATH_REDUCER_HASH_TABLE_LENGTH; ++i) {
        m_hash_store[i] = 0x00;
    }
}

UniquePathReducer::~UniquePathReducer() {
    for (unsigned int i = 0; i < UNIQUE_PATH_REDUCER_HASH_TABLE_LENGTH; ++i) {
        if (m_hash_store[i] != 0x00) {
            m_hash_store[i]->delete_elements();
            delete m_hash_store[i];
        }
    }
}

void UniquePathReducer::make_unique_file() {
    LinkedList<Path*> paths;

    ifstream input_file(m_context.paths_file().c_str());

    if (!input_file) {
        cout << "paths file does not exist: " << m_context.paths_file() << endl << flush;
        assert(false);
    }

    int buf_len = 16384;
    char buf[buf_len];
    char* ptr;

    while (!input_file.eof()) {
        input_file.getline(buf, buf_len);

        if (strlen(buf) == 0) {
            continue; 
        }

        nfa::Path* path = m_context.path_factory().new_path_from_external_string(buf);
        paths.push_back(path);

        add_path(path);
    }

    input_file.close();

    write_unique_path_file();

    paths.delete_elements();
}

void UniquePathReducer::write_unique_path_file()
{
    ofstream output_file(m_context.unique_paths_file().c_str());
    
    for (unsigned int i = 0; i < UNIQUE_PATH_REDUCER_HASH_TABLE_LENGTH; ++i) {
        if (m_hash_store[i] != 0x00) {
            for (LinkedList<Node*>::iterator it = m_hash_store[i]->begin(); it != m_hash_store[i]->end(); ++it) {
                output_file << (*it)->m_count << endl;
                output_file << (*it)->m_path->to_string() << endl;
            }
        }
    }

    output_file.close();
}

void UniquePathReducer::add_path(Path* path) {
    unsigned int key = compute_hash_key(*path);
    if (m_hash_store[key] == 0x00) {
        m_hash_store[key] = new LinkedList<Node*>();
    }
    
    for (LinkedList<Node*>::iterator it = m_hash_store[key]->begin(); it != m_hash_store[key]->end(); ++it) {
        if (*((*it)->m_path) == *path) {
            ++(*it)->m_count;
            return;
        }
    }

    Node* node = new Node(path, 1);
    m_hash_store[key]->push_back(node);
}

unsigned int UniquePathReducer::compute_hash_key(Path& path) {
    unsigned long hash_key = 0;
    for (unsigned int i = 0; i < path.length(); ++i) {
        hash_key += ((unsigned long)pow(i * path[i], path.length() - i) * 31);
    }

    return hash_key % UNIQUE_PATH_REDUCER_HASH_TABLE_LENGTH;
}

} //namespace nfa

