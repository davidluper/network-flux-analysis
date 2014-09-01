#ifndef FLUX_HISTOGRAM_H
#define FLUX_HISTOGRAM_H

#include "FluxSet.h"
#include "collections/LinkedList.h"
#include "Path.h"
#include <assert.h>

namespace nfa {

class FluxHistogram
{
public:
    FluxHistogram();
    FluxHistogram(FluxSet* flux_set);
    ~FluxHistogram();

    double& operator[](unsigned int i)
    {
        return m_histogram[i];
    }

    FluxHistogram& operator+=(const FluxHistogram& that)
    {
        assert((*this).m_flux_set->length() == that.m_flux_set->length());

        for (unsigned int i = 0; i < (*this).m_flux_set->length(); ++i)
        {
            (*this).m_histogram[i] += that.m_histogram[i];
        }

        return *this;
    }

    FluxHistogram& operator*=(int multiplier)
    {
        for (unsigned int i = 0; i < (*this).m_flux_set->length(); ++i)
        {
            (*this).m_histogram[i] *= multiplier;
        }

        return *this;
    }

    void initialize(FluxSet* flux_set);
    void from_labeled_paths(LinkedList<Path*>& reduced_flux_labeled_paths);
    
private:
    FluxSet* m_flux_set;
    double* m_histogram;
};

}

#endif
