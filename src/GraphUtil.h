#ifndef _GRAPHUTIL_H
#define _GRAPHUTIL_H

#include <vector>
#include <iostream>
#include <fstream>
#include "Util.h"
#include <assert.h>
#include <typeinfo>
#include <stdlib.h>
#include <cstring>

using namespace std;

namespace nfa {

#define UNWEIGHTED_GRAPH_DATA_TYPE unsigned char
#define WEIGHTED_GRAPH_DATA_TYPE long long
#define DOUBLE_GRAPH_DATA_TYPE double

typedef vector<vector<UNWEIGHTED_GRAPH_DATA_TYPE> > UnweightedGraph;
typedef vector<vector<WEIGHTED_GRAPH_DATA_TYPE> > WeightedGraph;
typedef vector<vector<double> > WeightedGraphD;

class GraphUtil
{
public:
    static void save_unweighted_graph(const char* filename, UnweightedGraph& graph);
    static void save_weighted_graph(const char* filename, WeightedGraph& graph);

    static void load_unweighted_graph_from_file(const char* filename, UnweightedGraph& graph);
    static void load_weighted_graph_from_file(const char* filename, WeightedGraph& graph);
    static void load_unweighted_graph_from_file(const char* filename, UnweightedGraph& graph, int number_of_compartments);
    static void load_weighted_graph_from_file(const char* filename, WeightedGraph& graph, int number_of_compartments);

    static void init_unweighted_graph(const char* filename, UnweightedGraph& graph);
    
    static UnweightedGraph* init_unweighted_graph_to_zeroes(int n);
    static WeightedGraph* init_weighted_graph_to_zeroes(int n);
    static UnweightedGraph* init_unweighted_graph_to_zeroes(const char* filename);
    static WeightedGraph* init_weighted_graph_to_zeroes(const char* filename); 
    
    template <typename T>
    static void print_graph(vector<vector<T> >& graph)
    {
        for (unsigned int i = 0; i < graph.size(); ++i)
        {
            for (unsigned int j = 0; j < graph.size(); ++j)
            {
                cout << (int)graph[i][j] << " ";
            }

            cout << endl;
        }
    }

    template <typename T>
    static void copy_graph(vector<vector<T> >& graph_src, vector<vector<T> >& graph_dest)
    {
        graph_dest.resize(graph_src.size());
        for (unsigned int i = 0; i < graph_src.size(); ++i)
        {
            graph_dest[i].resize(graph_src[i].size());
        }

        for (unsigned int i = 0; i < graph_src.size(); ++i)
        {
            for (unsigned int j = 0; j < graph_src[i].size(); ++j)
            {
                graph_dest[i][j] = graph_src[i][j];
            }
        }
    }
    
    template <typename T>
    static void init_graph_to_zeroes(vector<vector<T> >& graph, unsigned int rows_and_cols)
    {
        init_graph_to_zeroes(graph, rows_and_cols, rows_and_cols);
    }

    template <typename T>
    static void init_graph_to_zeroes(vector<vector<T> >& graph, unsigned int rows, unsigned int cols)
    {
        if (graph.size() != rows)
        {
            graph.resize(rows);
        }

        for (int i = 0; i < rows; i++)
        {
            graph[i].resize(cols);
        }
        
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                graph[i][j] = (T)0;
            }
        }
    }

    template <typename T>
    static void resize_graph(vector<vector<T> >& graph, unsigned int size)
    {
        graph.resize(size);
        for (unsigned int i = 0; i < size; ++i)
        {
            graph[i].resize(size, 0);
        }
    }

    static void convert_weighted_graph_to_unweighted_graph(WeightedGraph& weighted_graph, UnweightedGraph& unweighted_graph);

private:
    static int get_number_of_compartments_in_graph(const char* filename);
    
    template <typename T>
    static void save_graph(vector<vector<T> >& graph, const char* filename)
    {
        ofstream f(filename);
        for (int i = 0; i < graph.size(); i++)
        {
            for (int j = 0; j < graph.size(); j++)
            {
                if (typeid(T) == typeid(unsigned char))
                {
                    f << (int)graph[i][j];                    
                }
                else
                {               
                    f << graph[i][j];
                }
                if (j < graph.size() - 1) { f << " "; }
            }
            f << endl;
        }
        
        f.close();
    }

    template <typename T>
    static vector<vector<T> >* init_graph_to_zeroes(const char* filename)
    {
        vector<vector<T> >* graph = new vector<vector<T> >();
        init_graph_to_zeroes<T>(*graph, get_number_of_compartments_in_graph(filename));
        return graph;
    }

    template <typename T>
    static void load_graph_from_file(const char* filename, vector<vector<T> >& graph, int number_of_compartments)
    {
        if (number_of_compartments == -1)
        {
            number_of_compartments = get_number_of_compartments_in_graph(filename);
        }

        if (graph.size() != number_of_compartments)
        {
            graph.resize(number_of_compartments);
        }

        ifstream f (filename);

        if (!f)
        {
            cout << "graph file does not exist: " << filename << endl << flush;
        }

        char buf[1024];
        char* ptr;
        unsigned int ui_temp; 
        long long ll_temp;
        int i = 0, j = 0;
        while (!f.eof() && i < number_of_compartments)
        {
            if(graph[i].size() != number_of_compartments)
            {
                graph[i].resize(number_of_compartments);
            }
        
            f.getline(buf, 1023);

            if (strlen(buf) == 0)
            {
                continue;
            }

            ptr = strtok(buf, " ");
            j = 0;
            while (ptr != 0x00 && j < number_of_compartments)
            {
                if (!Util::is_numeric(ptr))
                {
                    ptr = strtok(0x00, " ");
                    continue;
                }

                if (typeid(T) == typeid(unsigned char))
                {
                    sscanf(ptr, "%u", &ui_temp);
                    graph[i][j] = (ui_temp == 0 ? 0 : 1);
                }
                else if (typeid(T) == typeid(long long))
                {
                    sscanf(ptr, "%lld", &ll_temp);
                    graph[i][j] = ll_temp;
                }
                else 
                {
                    cout << "invalid data type for graph" << endl << flush;
                    assert(false);
                }

                ptr = strtok(0x00, " ");
                j += 1;
            }

            i += 1;
        }
        
        f.close();
    }

};
} // namespace nfa

#endif
