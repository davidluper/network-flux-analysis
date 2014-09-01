#ifndef PATH_FACTORY_ECO_NET_H_
#define PATH_FACTORY_ECO_NET_H_

#include "PathFactory.h"
#include "FluxSet.h"
#include "FluxHistogram.h"
#include "GraphUtil.h"
#include <vector>

namespace nfa {

struct Edge {
    unsigned int u;
    unsigned int v;
};

class PathFactoryEcoNet : public PathFactory {
public:
    Path* new_path_from_external_string(char* s);
    void write_output(FluxHistogram& flux_histogram, FluxSet& flux_set, 
                      UnweightedGraph& graph, const std::string& coefficients_file, 
                      const std::string& output_directory);
    
private:
    unsigned int get_number_of_compartments_in_external_path_string(char* s);
    void load_external_path(char* s, Path& path);
    void write_graph(vector<vector<unsigned char> >& graph, std::ostream& out);
    void find_edge_for_stoichiometric_column(Edge& edge, 
                                             vector<vector<int> >& stoichiometric_matrix, 
                                             unsigned int column);
    void load_stoichiometric_matrix_from_file(const char* filename, 
                                              vector<vector<int> >& stoichiometric_matrix);
    void order_output_by_flux_histogram_pointers(unsigned int* flux_histogram_pointers, 
                                                 FluxHistogram& flux_histogram, 
                                                 vector<vector<unsigned char> >& flow_matrix);
    void sort_flux_histogram_pointers(unsigned int* flux_histogram_pointers, 
                                      FluxHistogram& flux_histogram, int l, int r);
};

} //namespace nfa

#endif
