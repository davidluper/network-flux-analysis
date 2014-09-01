#ifndef PATH_FACTORY_H_
#define PATH_FACTORY_H_

#include "NfaIncludes.h"
#include "Path.h"
#include "FluxSet.h"
#include "FluxHistogram.h"
#include "GraphUtil.h"
#include <string>

namespace nfa {

class PathFactory
{
public:
    virtual ~PathFactory();
    virtual Path* new_path_from_external_string(char* s) = 0;
    virtual void write_output(FluxHistogram& flux_histogram, FluxSet& flux_set, UnweightedGraph& graph, const std::string& coefficients_file, const std::string& output_directory);
    
    Path* new_path_from_internal_string(char* s);
    unsigned int get_number_of_compartments_in_internal_path_string(char* s);
    void load_internal_path(char* s, Path& path);
    Path* new_empty_path(unsigned int length);
};

} //namespace nfa

#endif
