#include "StructuralDecomposer.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "GraphUtil.h"

using namespace std;

namespace nfa {

StructuralDecomposer::StructuralDecomposer(RuntimeContext& context) :
    m_context(context)
{
    m_n_vertices = 0;             // number of vertices
    m_start = 0;                 // starting vertex index
    m_n_circuits = 0;             // total number of circuits found;
    m_len_longest = 0;            // its length
    m_enumeration = true;        // explicitly enumerate circuits
    m_stack_top = 0;              // the number of elements on the stack
}

void StructuralDecomposer::find_all_cycles(UnweightedGraph& graph, vector<vector<int> >& cycles)
{
    setup_globals(graph);
    stack_clear();
    m_start = 0;
    bool verbose = false;
    while (m_start < m_n_vertices) 
    {
        if (verbose && m_enumeration) 
        {
            printf("Starting s = %d\n", m_start); 
        }
        
        for (int i = 0; i < m_n_vertices; i++) 
        { 
            // for all i in Vk
            m_blocked[i] = false;
            empty_list(m_B[i]);
        }

        circuit(m_start, cycles);
        m_start = m_start + 1;
    }        
}

void StructuralDecomposer::decompose()
{
    vector<vector<int> > cycles;

    find_all_cycles(m_context.m_graph, cycles);

    ofstream f(m_context.cycles_file().c_str());
    
    for (int i = 0; i < cycles.size(); i++)
    {
        for (int j = 0; j < cycles[i].size(); j++)
        {
            f << cycles[i][j] << (j == cycles[i].size() - 1 ? "\n" : " ");
        }
    }
                                                                                                                                                                                   
    f.close();
}

// return TRUE if value is NOT in the list
bool StructuralDecomposer::not_in_list (vector<int>& vec, int val) 
{
    assert(vec[0] < vec.size());
    for (int i = 1; i <= vec[0]; i++) 
    {
        if (vec[i] == val) { return false; }
    }
    
    return true;
}

// empties a list by simply zeroing its size
void StructuralDecomposer::empty_list (vector<int>& vec) 
{
    assert(vec[0] < vec.size());
    vec[0] = 0;
}

// adds on to the end (making extra space if needed)
void StructuralDecomposer::add_to_list (vector<int>& vec, int val) 
{
    assert(vec[0] < vec.size());
    
    int new_pos = vec[0] + 1;
    if (new_pos >= vec.size())
    {
        vec.push_back(val);
    }
    else
    {
        vec[new_pos] = val;
    }
    
    vec[0] = new_pos;
}

// removes all occurences of val in the list
int StructuralDecomposer::remove_from_list(vector<int>& vec, int val) 
{
  assert(vec[0] < vec.size());
  int n_occurrences = 0;
  for (int i = 1; i <= vec[0]; i++) 
  {
      if (vec[i] == val) 
      {
          n_occurrences++;
          for (int j = i; j < vec[0]; j++) { vec[j] = vec[j + 1]; }
          --vec[0]; // should be safe as list[0] is
                    //re-evaluated each time around the i-loop
          --i;
      }
  }

  return n_occurrences;
}

void StructuralDecomposer::stack_print_3d(vector<vector<int> >& cycles) 
{
  cycles.push_back(vector<int>(m_stack_top));

  for (int i = 0; i < m_stack_top; i++) 
    {
      cycles[cycles.size() - 1][i] = m_stack[i];
    }
}

void StructuralDecomposer::unblock(int u) 
{
    m_blocked[u] = false;
    for (int w_pos = 1; w_pos <= m_B[u][0]; w_pos++) 
    {
        // for each w in B[u]
        int w = m_B[u][w_pos];
        w_pos -= remove_from_list(m_B[u], w);
        if (m_blocked[w])
        {
            unblock(w);
        }
    }
}

// initialise the stack to some size max
void StructuralDecomposer::stack_init() 
{
    m_stack_top = 0;
}

// push an int onto the stack, extending if necessary
void StructuralDecomposer::stack_push(int val) 
{
    m_stack.push_back(val);
    m_stack_top++;
}

int StructuralDecomposer::stack_size() 
{ 
    return m_stack_top; 
}

int StructuralDecomposer::stack_pop() 
{
    // pop an int off the stack
    assert(m_stack_top > 0);
    int rtn = m_stack[--m_stack_top];
    m_stack.pop_back();
    return rtn;
}

void StructuralDecomposer::stack_clear () 
{
    // clear the stack
    m_stack_top = 0;
    while (m_stack.size() > 0) 
    {
        m_stack.pop_back(); 
    }
}

void StructuralDecomposer::stack_duplicate(vector<int>& stk, vector<int>& other)
{
    other.clear();
    for (int i = 0; i < stk.size(); i++)
    {
        other.push_back(stk[i]);
    }//for i
}

bool StructuralDecomposer::circuit(int v, vector<vector<int> >& cycles) 
{ // based on Johnson’s logical procedure CIRCUIT
    bool f = false;
    stack_push(v);
    m_blocked[v] = true;

    for (int w_pos = 1; w_pos <= m_Ak[v][0]; w_pos++) 
    { // for each w in list Ak[v]:
        int w = m_Ak[v][w_pos];
        if (w < m_start) 
        { 
            // ignore relevant parts of Ak
            continue; 
        }

        if (w == m_start) 
        { 
            // we have a circuit,
            if (m_enumeration) 
            {
                // print out the stack to record the circuit
                stack_print_3d(cycles); 
            }

            assert (m_stack_top <= m_n_vertices);
            ++m_length_histogram[m_stack_top]; // add this circuit ’s length to the length histogram
            m_n_circuits++; // and increment count of circuits found

            if (m_stack_top > m_len_longest) 
            { 
                // keep a copy of the longest circuit found
                m_len_longest = m_stack_top;
                //longestCircuit = stack.dup;
                stack_duplicate(m_stack, m_longest_circuit);
            }

            for (int i = 0; i < m_stack_top; i++) 
            {// increment [circuit-length][vertex] for all vertices in this circuit
                ++m_vertex_popularity[m_stack_top][m_stack[i]];
            }

            f = true;
        } 
        else if (!m_blocked[w]) 
        {
            if (circuit(w, cycles)) 
            {
                f = true;
            }
        }
    }

    if (f) 
    {
        unblock(v); 
    } 
    else 
    {
        for (int w_pos = 1; w_pos <= m_Ak[v][0]; w_pos++) 
        { // for each w in list Ak[v]:
            int w = m_Ak[v][w_pos];
            if (w < m_start) 
            {
                continue; 
            }  // ignore relevant parts of Ak
            
            if (not_in_list(m_B[w], v)) 
            {
                add_to_list(m_B[w], v); 
            }
        }
    }
    
    v = stack_pop();
    return f;
}

void StructuralDecomposer::setup_globals(UnweightedGraph& A) 
{  // presupposes nVertices is set up
    if (A.size() <= 0) 
    {
        cout << "empty input graph" << endl; 
        return; 
    }
    
    m_n_vertices = A[0].size();
    if (m_n_vertices <= 0) 
    { 
        cout << "graph has no vertices" << endl; 
        return; 
    }
    
    init_vec(m_Ak, m_n_vertices); // Ak[i][0] is the number of members, Ak[i][1]..Ak[i][n] ARE the members, i>0
    init_vec(m_B, m_n_vertices);  // B[i][0] is the number of members, B[i][1]..B[i][n] ARE the members , i>0
    init_vec(m_blocked, m_n_vertices); // we use blocked [0]..blocked[n-1], i> = 0
    for (int i = 0; i < m_n_vertices; i++) 
    {
        init_vec(m_Ak[i], m_n_vertices + 1);
        init_vec(m_B[i], m_n_vertices + 1);
        m_blocked[i] = false;
    }
    
    for (int i = 0; i < A.size(); i++) 
    {
        for (int j = 0; j < A[0].size(); j++)
        { 
            if (A[i][j] > 0)
            {
                add_to_list(m_Ak[i], j);
            }
        }
    }
    
    // will use as [1]...[n] to histogram circuits by length
    init_vec(m_length_histogram,  m_n_vertices + 1); 
    // [0] for zero length circuits, which are impossible
    for (int len = 0; len < m_length_histogram.size(); len++) // initialise histogram bins to empty
    { 
        m_length_histogram[len] = 0;
    }//for len
    
    stack_init();
    init_vec(m_vertex_popularity, m_n_vertices + 1); // max elementary circuit length is exactly nVertices
    for (int len = 0; len <= m_n_vertices; len++) 
    {
        init_vec(m_vertex_popularity[len], m_n_vertices);
        for (int j = 0; j < m_n_vertices; j++) 
        {
            m_vertex_popularity[len][j] = 0;
        }
    }
}

} //namespace nfa
