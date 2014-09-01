#include "DecompositionPlanExecutor.h"
#include "UniquePathReducer.h"
#include "AdjacencyMatrixBuilder.h"
#include "GraphUtil.h"
#include "StructuralDecomposer.h"
#include "FunctionalDecomposer.h"
#include "PathFactory.h"
#include <iomanip>

namespace nfa {

DecompositionPlanExecutor::DecompositionPlanExecutor(RuntimeContext& context) :
    m_context(context) {
    
}

void DecompositionPlanExecutor::execute_plan() {
    //make a paths file that has only unique paths on every other line
    //and the number of times it occured on the other lines
    UniquePathReducer unique_path_reducer(m_context);
    unique_path_reducer.make_unique_file();

    if (m_context.do_build_adjacency_matrix()) {
        //analyze the paths.dat file to build an adjacency matrix from 
        //symbol transitions in the paths.dat file
        AdjacencyMatrixBuilder adjacency_matrix_builder(m_context);
        adjacency_matrix_builder.build_adjacency_matrix();
    } else {
        //load an existing adjacency matrix file from somewhere on disk
        GraphUtil::init_unweighted_graph(m_context.adjacency_matrix_file().c_str(), m_context.m_graph);
        GraphUtil::load_unweighted_graph_from_file(m_context.adjacency_matrix_file().c_str(), 
                                                   m_context.m_graph, m_context.m_graph.size());
    }

    //structurally decompose matrix to make cycles.out
    StructuralDecomposer structural_decomposer(m_context);
    structural_decomposer.decompose();

    //functionally decompose to produce output file(s) (coefficients.vector)
    FunctionalDecomposer functional_decomposer(m_context);
    functional_decomposer.decompose();

    //write the output files to disk
    m_context.path_factory().write_output(m_context.m_flux_histogram, m_context.m_flux_set, 
                                          m_context.m_graph, m_context.coefficients_file(), 
                                          m_context.directory());
}

} //namespace nfa
