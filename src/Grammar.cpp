#include "Grammar.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;

namespace nfa {

unsigned int Grammar::literals_ndx() const
{
    return m_literals_ndx;
}

unsigned int Grammar::literals_length() const
{
    return m_length_literals;
}

unsigned long Grammar::get_grammar_size(UnweightedGraph& graph, FluxSet& flux_set)
{
    unsigned int grammar_size = graph.size();
    for (int i = 0; i < flux_set.length(); i++)
    {
        if (flux_set[i].is_chain())
        {
            grammar_size += (flux_set[i].length() - 1);
        }
        else
        {
            grammar_size += (flux_set[i].length() * (flux_set[i].length() - 1));
        }
    }
    
    //account for the literals at the end of the grammar
    grammar_size += graph.size();
    
    return grammar_size;
}

Grammar::Grammar()
{

}

Grammar::~Grammar()
{
    m_rules.delete_elements();
    for (int i = 0; i < m_length_productions; ++i)
    {
        m_productions[i].delete_elements();
    }
    
    delete[] m_productions;
    delete[] m_flux_ids;
}

void Grammar::initialize(UnweightedGraph& graph, FluxSet& flux_set)
{
    m_length_productions = get_grammar_size(graph, flux_set);
    m_productions = new LinkedList<ProductionRightHandSide*>[m_length_productions];
    m_flux_ids = new COMPARTMENT_DATA_TYPE[m_length_productions];
    
    for (int i = 0; i < m_length_productions; i++)
    {
        m_flux_ids[i] = -1;
    }
}

COMPARTMENT_DATA_TYPE Grammar::get_flux_id_for_production(unsigned int production_ndx)
{
    return m_flux_ids[production_ndx];
}

unsigned int Grammar::total_number_of_productions() const
{
    return m_length_productions; 
}
    
void Grammar::save_grammar()
{
    ofstream f("./grammar.out");
    
    for (int i = 0; i < m_length_nonliterals; i++)
    {
        f << i << ">";

        for (LinkedList<ProductionRightHandSide*>::iterator it = productions_nonliterals(i).begin(); it != productions_nonliterals(i).end(); ++it)
        {
            f << (int)(*it)->B << " " << (int)(*it)->C << " ";
        }
        
        f << endl;
    }

    for (int i = 0; i < m_length_literals; i++)
    {
        f << (m_literals_ndx + i) << ">";

        for (LinkedList<ProductionRightHandSide*>::iterator it = productions_literals(i).begin(); it != productions_literals(i).end(); ++it)
        {
            f << (int)(*it)->B << " " << (int)(*it)->C << " ";
        }

        f << endl;
    }
    
    f.close();  
}

void Grammar::build_grammar_rules(FluxSet& flux_set)
{
    m_rules.clear();
    
    for (int i = 0; i < flux_set.length(); i++)
    {      
        if (flux_set[i].is_chain())
        {
            m_rules.push_back(flux_set[i].copy());
            m_rules.tail()->set_last_compartment((*m_rules.tail())[0]);
        }
        else
        {
            for (int j = 0; j < flux_set[i].length(); j++)
            {
                m_rules.push_back(flux_set[i].copy());
                m_rules.tail()->set_last_compartment((*m_rules.tail())[0]);
                flux_set[i].carousel_op();
            }
        }
    }
}

void Grammar::transform_to_chomsky(UnweightedGraph& graph)
{
    m_literals_ndx = m_length_productions - graph.size();    
    m_length_literals = m_length_productions - m_literals_ndx;
    m_length_nonliterals = m_length_productions - m_length_literals;

    int new_production_counter = graph.size();
    
    assert(m_rules.size() > 0); 
    
    for (LinkedList<Flux*>::iterator it = m_rules.begin(); it != m_rules.end(); ++it)
    {   
        productions_nonliterals((**it)[0]).push_back(new ProductionRightHandSide((**it)[0], new_production_counter));        
        
        for (int ndx = 1; ndx < (**it).length() - 2; ndx++)
        {
            new_production_counter += 1;
            productions_nonliterals(new_production_counter - 1).push_back(new ProductionRightHandSide((**it)[ndx], new_production_counter));
            m_flux_ids[new_production_counter - 1] = (**it).flux_id();
        }
        
        productions_nonliterals(new_production_counter).push_back(new ProductionRightHandSide((**it)[(**it).length() - 2], (**it)[(**it).length() - 1]));
        m_flux_ids[new_production_counter] = (**it).flux_id();
        new_production_counter += 1;
    } 
    
    for (int i = 0; i < graph.size(); i++)
    {
        productions_literals(i).push_back(new ProductionRightHandSide(i, -i));
    }
}

} // namespace nfa
