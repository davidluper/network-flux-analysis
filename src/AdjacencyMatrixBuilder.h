#ifndef ADJACENCY_MATRIX_BUILDER_H
#define ADJACENCY_MATRIX_BUILDER_H

#include "RuntimeContext.h"
#include "GraphUtil.h"

namespace nfa {

class AdjacencyMatrixBuilder {
public:
    AdjacencyMatrixBuilder(RuntimeContext& context);

    void build_adjacency_matrix();

private:
    RuntimeContext& m_context;
};

} //namespace

#endif
