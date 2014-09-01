#include "FluxHistogram.h"

namespace nfa {

FluxHistogram::FluxHistogram() {
    m_histogram = 0x00;
}

FluxHistogram::FluxHistogram(FluxSet* flux_set) {
    m_histogram = 0x00;
    initialize(flux_set);
}

/*
 * initialize from a set of fluxes, zero out the histogram and set the local
 * flux set pointer to point to the supplied flux_set
 */
void FluxHistogram::initialize(FluxSet* flux_set) {
    if (m_histogram != 0x00) {
        delete[] m_histogram;
    }

    m_flux_set = flux_set;
    m_histogram = new double[m_flux_set->length()];
    for(unsigned int i= 0; i < m_flux_set->length(); ++i) {
        m_histogram[i] = 0.0;
    }    
}

FluxHistogram::~FluxHistogram() {
    delete[] m_histogram;
}

/*
 * this function initializes a flux histogram from a reduced set of labeled paths
 */
void FluxHistogram::from_labeled_paths(LinkedList<Path*>& reduced_flux_labeled_paths) {
    for (int i = 0; i < m_flux_set->length(); ++i) {
        m_histogram[i] = 0.0;
    }

    for (LinkedList<Path*>::iterator it = reduced_flux_labeled_paths.begin(); 
         it != reduced_flux_labeled_paths.end(); ++it) {
        //start at index 1 because index 0 will always have a value of -1
        for (unsigned int i = 1; i < (*it)->length(); ++i) {
            ++m_histogram[(*(*it))[i]];
        }
    }

    //we only want to count each occurence of a flux once and the for loops above
    //incremented the histogram for each occurence of the flux label id.  also we
    //we want the ratio of how many times the flux appeared as a ratio to how many 
    //ambiguous interpretations of the flux there was (so if there are two interpretations
    //and a flux appears twice it should have a value of 1 in the histogram, if a flux
    //appears once and there are two interpretations the flux should have a value of
    //0.5). 
    //
    //eg. a flux that is three elements long with an id of 11 that occured once when 
    //there were two interpretations of a path,  would have incremented the histogram 
    //cell 11 three times.  the histogram cell 11 should have a value of 0.5
    //
    // here are the interpretations
    // -1 11 11 11 8 8 5 5 5
    // -1 3  3  5  5 5 4 4 4
    //
    // 11 happens once and there are two interpretations so it should have a value of 0.5
    // 3 / (3 * 2) = 0.5
    for (unsigned int i = 0; i < m_flux_set->length(); ++i) {
        m_histogram[i] /= ((double)(*m_flux_set)[i].length() * 
                           (double)reduced_flux_labeled_paths.size());
    }
}

} //namespace nfa
