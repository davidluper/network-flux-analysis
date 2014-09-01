#ifndef FLUXSET_H
#define FLUXSET_H

#include "Flux.h"
#include "GraphUtil.h"
#include <vector>
#include <string>

using namespace std;

namespace nfa {

class Grammar;

class FluxSet
{
public:
    FluxSet();
    ~FluxSet();

    void create_grammar(UnweightedGraph& graph, Grammar& grammar);
    void load_fluxes(const std::string& cycles_file);
    unsigned int length() const;

    Flux& operator[] (unsigned int i);
    const Flux& operator[] (unsigned int i) const;

private:
    Flux* m_fluxes;
    unsigned int m_length;
    
    void expand_cycles();
};

} // namespace nfa

#endif
