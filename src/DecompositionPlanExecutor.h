#ifndef DECOMPOSITION_PLAN_H
#define DECOMPOSITION_PLAN_H

#include "RuntimeContext.h"

namespace nfa {

class DecompositionPlanExecutor {
public:
    DecompositionPlanExecutor(RuntimeContext& context);
    void execute_plan();

private:
    RuntimeContext& m_context;
};

} //namespace nfa

#endif
