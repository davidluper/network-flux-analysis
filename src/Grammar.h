#ifndef _GRAMMAR_H
#define _GRAMMAR_H

#include "collections/LinkedList.h"
#include <vector>
#include "Flux.h"
#include "FluxSet.h"
#include "GraphUtil.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;

namespace nfa {

class FluxSet;

// LEFT HAND SIDE     ->    RIGHT HAND SIDE
// A                  ->    B   C
class ProductionRightHandSide
{
public:
    ProductionRightHandSide(const ProductionRightHandSide& other) :
        B(0),
        C(0)
    {
        cout << "don't call ProductionRightHandSide copy constructor" << endl;
        assert(false);
    }

ProductionRightHandSide(COMPARTMENT_DATA_TYPE B, COMPARTMENT_DATA_TYPE C) : B(B), C(C) { };
    bool is_terminal() { return B < 0 || C < 0; }
    
    const COMPARTMENT_DATA_TYPE B;
    const COMPARTMENT_DATA_TYPE C;

    void print()
    {
        cout << B << " " << C << " $ ";
    }

    std::string to_string()
    {
        stringstream ss;
        ss << B << " " << C;
        return ss.str();
    }
    
};

class Grammar
{
friend class FluxSet;
    
public:    
    class iterator_nonliterals
    {
    private:
        unsigned int m_ndx;
        Grammar* m_grammar;
    public:
        iterator_nonliterals(unsigned int ndx, Grammar* grammar)
        {
            m_ndx = ndx;
            m_grammar = grammar;
        }
        
        iterator_nonliterals& operator=(const iterator_nonliterals& other)
            {
                m_ndx = other.m_ndx;
                m_grammar = other.m_grammar;
                return(*this);
            }

        unsigned int get_production_ndx() const 
        {
            return m_ndx;
        }
        
        bool operator==(const iterator_nonliterals& other)
        {
            return m_ndx == other.m_ndx;
        }
        
        bool operator!=(const iterator_nonliterals& other)
        {
            return m_ndx != other.m_ndx;
        }
        
        iterator_nonliterals& operator++()
        {
            if (m_ndx != m_grammar->m_length_nonliterals)
            {
                ++m_ndx;
            }
            return(*this);
        }
        
        iterator_nonliterals operator++(int)
        {
            iterator_nonliterals tmp(*this);
            ++(*this);
            return tmp;
        }
        
        LinkedList<ProductionRightHandSide*>& operator*()
        {
            return m_grammar->m_productions[m_ndx];
        }
        
        LinkedList<ProductionRightHandSide*>* operator->()
        {
            return(&*(Grammar::iterator_nonliterals)*this);
        }
        
    };

    class iterator_literals
    {
    private:
        unsigned int m_ndx;
        Grammar* m_grammar;
    public:
        iterator_literals(unsigned int ndx, Grammar* grammar)
        {
            m_grammar = grammar;
            m_ndx = ndx;
        }
        
        iterator_literals& operator=(const iterator_literals& other)
            {
                m_ndx = other.m_ndx;
                m_grammar = other.m_grammar;
                return(*this);
            }
        
        unsigned int get_production_ndx() const 
        {
            return m_ndx;
        }

        bool operator==(const iterator_literals& other)
        {
            return m_ndx == other.m_ndx;
        }
        
        bool operator!=(const iterator_literals& other)
        {
            return m_ndx != other.m_ndx;
        }
        
        iterator_literals& operator++()
        {
            if (m_ndx != m_grammar->m_length_productions)
            {
                ++m_ndx;
            }
            return(*this);
        }
        
        iterator_literals operator++(int)
        {
            iterator_literals tmp(*this);
            ++(*this);
            return tmp;
        }
        
        LinkedList<ProductionRightHandSide*>& operator*()
        {
            return m_grammar->m_productions[m_ndx];
        }
        
        LinkedList<ProductionRightHandSide*>* operator->()
        {
            return(&*(Grammar::iterator_literals)*this);
        }
        
    };

    iterator_nonliterals begin_nonliterals() { return iterator_nonliterals(0, this); }
    iterator_nonliterals end_nonliterals() { return iterator_nonliterals(m_length_nonliterals, this); }
    iterator_literals begin_literals() { return iterator_literals(m_literals_ndx, this); }
    iterator_literals end_literals() { return iterator_literals(m_length_productions, this); }


    LinkedList<ProductionRightHandSide*>& productions_nonliterals(unsigned int ndx)
    {
        assert(ndx < m_length_nonliterals);
        return m_productions[ndx];
    }

    LinkedList<ProductionRightHandSide*>& productions_literals(unsigned int ndx)
    {
        assert(ndx < m_length_literals);
        return m_productions[m_literals_ndx + ndx];
    }

    unsigned int total_number_of_productions() const;
    unsigned int literals_ndx() const;
    unsigned int literals_length() const;

    void save_grammar();
    COMPARTMENT_DATA_TYPE get_flux_id_for_production(unsigned int production_ndx);

    Grammar();
    ~Grammar();

private:
    void initialize(UnweightedGraph& graph, FluxSet& flux_set);
    unsigned long get_grammar_size(UnweightedGraph& graph, FluxSet& flux_set);
    void build_grammar_rules(FluxSet& flux_set);
    void transform_to_chomsky(UnweightedGraph& graph);

    LinkedList<ProductionRightHandSide*>* m_productions;
    LinkedList<Flux*> m_rules;
    
    unsigned int m_literals_ndx;
    unsigned int m_length_productions;
    unsigned int m_length_nonliterals;
    unsigned int m_length_literals;

    COMPARTMENT_DATA_TYPE* m_flux_ids;

};

} // namespace nfa

#endif
