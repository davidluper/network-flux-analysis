#include "GraphUtil.h"
#include "Path.h"
#include "PathFactory.h"

using namespace std;

namespace nfa {

void GraphUtil::convert_weighted_graph_to_unweighted_graph(WeightedGraph& weighted_graph, UnweightedGraph& unweighted_graph)
{
    resize_graph(unweighted_graph, weighted_graph.size());
    
    for (unsigned int i = 0; i < weighted_graph.size(); ++i)
    {
        for (unsigned int j = 0; j < weighted_graph.size(); ++j)
        {
            unweighted_graph[i][j] = weighted_graph[i][j] > 0 ? 1 : 0;
        }
    }
}

void GraphUtil::save_unweighted_graph(const char* filename, UnweightedGraph& graph)
{
    save_graph<UNWEIGHTED_GRAPH_DATA_TYPE>(graph, filename);
}

void GraphUtil::save_weighted_graph(const char* filename, WeightedGraph& graph)
{
    save_graph<WEIGHTED_GRAPH_DATA_TYPE>(graph, filename);
}

UnweightedGraph* GraphUtil::init_unweighted_graph_to_zeroes(const char* filename)
{
    return init_graph_to_zeroes<UNWEIGHTED_GRAPH_DATA_TYPE>(filename);
}

WeightedGraph* GraphUtil::init_weighted_graph_to_zeroes(const char* filename)
{
    return init_graph_to_zeroes<WEIGHTED_GRAPH_DATA_TYPE>(filename);
}

UnweightedGraph* GraphUtil::init_unweighted_graph_to_zeroes(int n)
{
    UnweightedGraph* graph = new UnweightedGraph(n);
    init_graph_to_zeroes<UNWEIGHTED_GRAPH_DATA_TYPE>(*graph, n);
    return graph;
}

WeightedGraph* GraphUtil::init_weighted_graph_to_zeroes(int n)
{
    WeightedGraph* graph = new WeightedGraph(n);
    init_graph_to_zeroes<WEIGHTED_GRAPH_DATA_TYPE>(*graph, n);
    return graph;
}

int GraphUtil::get_number_of_compartments_in_graph(const char* filename)
{
    ifstream f(filename);

    if (!f)
    {
        cout << "input adjacency matrix file does not exist: " << filename << endl << flush;
        assert(false);
    }

    char buf[1024];
    
    int rtn = 0;
    while (!f.eof())
    {
        f.getline(buf, 1023);
        if (buf[0] == 0x00) { continue; }
        rtn += 1;
    }
    
    f.close();
    
    return rtn;
}

void GraphUtil::init_unweighted_graph(const char* filename, UnweightedGraph& graph)
{
    graph.resize(get_number_of_compartments_in_graph(filename));
    for (int i = 0; i < graph.size(); i++)
    {
        graph[i].resize(graph.size(), 0);
    }
}
    
void GraphUtil::load_unweighted_graph_from_file(const char* filename, UnweightedGraph& graph)
{
    load_unweighted_graph_from_file(filename, graph, -1);
}

void GraphUtil::load_unweighted_graph_from_file(const char* filename, UnweightedGraph& graph, int number_of_compartments)
{
    load_graph_from_file<UNWEIGHTED_GRAPH_DATA_TYPE>(filename, graph, number_of_compartments);
}

void GraphUtil::load_weighted_graph_from_file(const char* filename, WeightedGraph& graph)
{
    load_weighted_graph_from_file(filename, graph, -1);
}

void GraphUtil::load_weighted_graph_from_file(const char* filename, WeightedGraph& graph, int number_of_compartments)
{
    load_graph_from_file<WEIGHTED_GRAPH_DATA_TYPE>(filename, graph, number_of_compartments);
}

} // namespace nfa
