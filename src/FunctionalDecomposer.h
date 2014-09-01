#ifndef FUNCTIONAL_DECOMPOSER_H
#define FUNCTIONAL_DECOMPOSER_H

#include "GraphUtil.h"
#include "RuntimeContext.h"
#include "FluxSet.h"
#include "Grammar.h"
#include "Cyk.h"

namespace nfa {

class FunctionalDecomposer
{
public:
    FunctionalDecomposer(RuntimeContext& context);
    void decompose();
    
private:
    Grammar m_grammar;
    Cyk m_cyk;
    RuntimeContext& m_context;

    void initialize_decomposition();
    void interpret_paths();
};

} //namespace nfa

#endif
