#ifndef _STRUCTURALDECOMPOSER_H
#define _STRUCTURALDECOMPOSER_H

#include <vector>
#include "GraphUtil.h"
#include "RuntimeContext.h"

using namespace std;

/*
* Enumerating Circuits and Loops in Graphs with Self-Arcs and Multiple-Arcs
* K.A. Hawick and H.A. James
* Computer Science, Institute for Information and Mathematical Sciences,
* Massey University, North Shore 102-904, Auckland, New Zealand
* k.a.hawick@massey.ac.nz; heath.james@sapac.edu.au
* Tel: +64 9 414 0800
* Fax: +64 9 441 8181
* Technical Report CSTN-013
*/

namespace nfa
{
class StructuralDecomposer
{
public:
    StructuralDecomposer(RuntimeContext& context);
    void decompose();
    
private:
    RuntimeContext& m_context;
    int m_n_vertices;                                    // number of vertices
    int m_start;                                        // starting vertex index
    vector<vector<int> > m_Ak;                          // integer array size n of lists
                                                      // ie the arcs from the vertex
    vector<vector<int> > m_B;                           // integer array size n of lists
    vector<bool> m_blocked;                             // logical array indexed by vertex
    unsigned long m_n_circuits;                          // total number of circuits found;
    vector<unsigned long> m_length_histogram;            // histogram of circuit lengths
    vector<vector<unsigned long> > m_vertex_popularity;  // adjacency table of occurrences of
                                                      // vertices in circuits of each length
    vector<int> m_longest_circuit;                       // the (first) longest circuit found
    int m_len_longest;                                   // its length
    bool m_enumeration;                                 // explicitly enumerate circuits
    vector<int> m_stack;                                // stack of integers
    int m_stack_top;                                     // the number of elements on the stack
                                                      // also the index "to put the next one"

    // return a pointer to a list of fixed max size
    template <class T>
    void init_vec(vector<T>& vec, int len)
    {
        vec.clear();
        for (int i = 0; i < len; i++)
        {
            vec.push_back(0);
        }//for i
    }

    template <class T>
    void init_vec(vector<vector<T> >& vec, int len)
    {
        vec.clear();
        for (int i = 0; i < len; i++)
        {
            vec.push_back(vector<T>(0));
        }//for i
    }

    void find_all_cycles(UnweightedGraph& graph, vector<vector<int> >& cycles);
    bool not_in_list (vector<int>& vec, int val);
    void empty_list (vector<int>& vec);
    void add_to_list (vector<int>& vec, int val);
    int remove_from_list(vector<int>& vec, int val);
    void stack_print_3d(vector<vector<int> >& cycles);
    void unblock (int u);
    void stack_init();
    void stack_push (int val);
    int stack_size();
    int stack_pop ();
    void stack_clear();
    void stack_duplicate(vector<int>& stk, vector<int>& other);
    bool circuit(int v, vector<vector<int> >& cycles);
    void setup_globals(UnweightedGraph& A);
};
} //namespace nfa
#endif
