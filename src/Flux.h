#ifndef _FLUXUTIL_H
#define _FLUXUTIL_H

#include <iostream>
#include <fstream>
#include <string.h>
#include "NfaIncludes.h"

using namespace std;

namespace nfa {

/*
 * this class encapsilates a flux which is a cycle in a graph
 */
class Flux {
//this is just padding on the end of the compartments array
//its needed to close fluxes (make them stop where they start, graph cycle)
#define EXTRA_LENGTH 1
public:
    Flux();
    //Flux(int flux_id, unsigned int length);
    void init(int flux_id, unsigned int length);
    
    ~Flux();
    
    COMPARTMENT_DATA_TYPE& operator[](unsigned int ndx) {
        return m_compartments[ndx];
    };

    int flux_id() const;
    void set_flux_id(int flux_id);
    bool is_chain();
    void set_last_compartment(COMPARTMENT_DATA_TYPE val);    
    void write_to_stream(ofstream& f);
    Flux* copy();    
    void carousel_op();    
    unsigned int length() const;
    void set_length(unsigned int length);
    std::string to_string();

private:    
    unsigned int get_number_of_compartments_in_flux(char* s);
    void load_flux(char* s);

    COMPARTMENT_DATA_TYPE* m_compartments;
    unsigned int m_length;
    unsigned int m_total_length;
    int m_flux_id;
};
} //namespace nfa

#endif
