#include "PathFactory.h"
#include "Util.h"
#include <assert.h>

namespace nfa {

PathFactory::~PathFactory() {

}

Path* PathFactory::new_empty_path(unsigned int length) {
    Path* path = new Path(length);
    return path;
}

Path* PathFactory::new_path_from_internal_string(char* s) {
    Path* path = new Path(get_number_of_compartments_in_internal_path_string(s));
    load_internal_path(s, *path);

    return path;
}

unsigned int PathFactory::get_number_of_compartments_in_internal_path_string(char* s) {
    int len = 0;
    int i = 0;
    bool is_character = false;
    while (s[i] != 0x00) {
        if (!is_character && s[i] != ' ') {
            is_character = true;
            len += 1;
        }
        
        if (s[i] == ' ') {
            is_character = false;
        }
        
        ++i;
    }
    
    return len;
}

void PathFactory::load_internal_path(char* s, Path& path) {
    char* ptr = strtok(s, " ");
    int j = 0;
    COMPARTMENT_DATA_TYPE temp;
    while (ptr != 0x00) {
        assert(Util::is_numeric(ptr));
        sscanf(ptr, "%hi", &temp);
        path.m_compartments[j] = temp;
        
        ptr = strtok(0x00, " ");
        j += 1;
    }
}

void PathFactory::write_output(FluxHistogram& flux_histogram, FluxSet& flux_set, UnweightedGraph& graph, const std::string& coefficients_file, const std::string& output_directory) {
    ofstream flux_histogram_file(coefficients_file.c_str());
    for (unsigned int i = 0; i < flux_set.length(); ++i) {
        flux_histogram_file << flux_histogram[i] << std::endl;
    }

    flux_histogram_file.close();
}

} //namespace nfa
