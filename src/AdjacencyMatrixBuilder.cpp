#include "AdjacencyMatrixBuilder.h"
#include "Path.h"
#include "PathFactory.h"
#include <string>
#include <iostream>
#include "GraphUtil.h"
#include "Util.h"

using namespace std;
namespace nfa {

AdjacencyMatrixBuilder::AdjacencyMatrixBuilder(RuntimeContext& context) :
    m_context(context) {

}

/*
 * build an adjacency matrix from a file of paths. each line will be deserialized into
 * a path object by the current path factory.  once the line is in a path file edges are 
 * created in the adjacency matrix for PATH[i] -> PATH[i+1] so if there is only one path 
 * and its A B C B or 0 1 2 1 the adjacency matrix would look like
 *
 *    A B C
 * A  0 1 0
 * B  0 0 1 
 * C  0 1 0
 *
 */
void AdjacencyMatrixBuilder::build_adjacency_matrix() {
    GraphUtil::resize_graph(m_context.m_graph, 1);

    ifstream f (m_context.paths_file().c_str());
    
    if (!f) {
        cout << "paths file does not exist: " << m_context.paths_file() << endl << flush;
        assert(false);
    }

    int buf_len = 16384;
    char buf[buf_len];

    while (!f.eof()) {
        f.getline(buf, buf_len);
        
        if (buf[0] == 0x00) {
            continue;
        }

        Path* path = m_context.path_factory().new_path_from_external_string(buf);

        if ((*path)[0] >= m_context.m_graph.size()) {
            GraphUtil::resize_graph(m_context.m_graph, (*path)[0] + 1);
        }

        for (unsigned int i = 1; i < path->length(); ++i) {
            if ((*path)[i] >= m_context.m_graph.size()) {
                GraphUtil::resize_graph(m_context.m_graph, (*path)[i] + 1);
            }

            m_context.m_graph[(*path)[i - 1]][(*path)[i]] = 1;
        }

        delete path;
    }

    f.close();
    
    GraphUtil::save_unweighted_graph(m_context.adjacency_matrix_file().c_str(), m_context.m_graph);
}

} //namespace nfa
