#include "PathFactoryEcoNet.h"
#include "Util.h"
#include <assert.h>
#include "GraphUtil.h"

namespace nfa {

Path* PathFactoryEcoNet::new_path_from_external_string(char* s) {
    Path* path = new Path(get_number_of_compartments_in_external_path_string(s) + 2);
    load_external_path(s, *path);

    return path;
}

unsigned int PathFactoryEcoNet::get_number_of_compartments_in_external_path_string(char* s) {
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

void PathFactoryEcoNet::load_external_path(char* s, Path& path) {
    char* ptr = strtok(s, " ");
    int j = 1;
    COMPARTMENT_DATA_TYPE temp;
    while (ptr != 0x00) {
        assert(Util::is_numeric(ptr));
        sscanf(ptr, "%hi", &temp);
        path.m_compartments[j] = temp + 1;
        
        ptr = strtok(0x00, " ");
        j += 1;
    }

    path.m_compartments[0] = 0;
    path.m_compartments[j] = 0;
}

void PathFactoryEcoNet::load_stoichiometric_matrix_from_file(const char* filename, 
                                                             vector<vector<int> >& stoichiometric_matrix) {
    stoichiometric_matrix.clear();

    ifstream f (filename);
    
    if (!f) {
        cout << "stoichiometric matrix does not exist: " << filename << endl;
        assert(false);
    }

    char buf[16384];
    char* ptr;
    long long ll_val;
    while (!f.eof()) {        
        f.getline(buf, 16383);
        
        if (strlen(buf) == 0) {
            continue;
        }
        
        vector<int> row;

        ptr = strtok(buf, " ");
        while (ptr != 0x00) {
            if (!Util::is_numeric(ptr)) {
                ptr = strtok(0x00, " ");
                continue;
            }
            
            sscanf(ptr, "%lld", &ll_val);
            row.push_back(ll_val);
            
            ptr = strtok(0x00, " ");
        }
        
        stoichiometric_matrix.push_back(row);
    }
    
    f.close();
}

void PathFactoryEcoNet::find_edge_for_stoichiometric_column(Edge& edge, 
                                                            vector<vector<int> >& stoichiometric_matrix, 
                                                            unsigned int column) {
    edge.u = 0;
    edge.v = 0;
    for (int row = 0; row < stoichiometric_matrix.size(); ++row) {
        if (stoichiometric_matrix[row][column] == -1) {
            edge.u = row + 1;
        } else if (stoichiometric_matrix[row][column] == 1) {
            edge.v = row + 1;
        }
    }
}

void PathFactoryEcoNet::write_output(FluxHistogram& flux_histogram, FluxSet& flux_set, 
                                     UnweightedGraph& graph, const std::string& coefficients_file, 
                                     const std::string& output_directory) {
    vector<vector<int> > stoichiometric_matrix;
    std::string stoichiometric_matrix_file = output_directory + "/stoichiometric.matrix"; 
    load_stoichiometric_matrix_from_file(stoichiometric_matrix_file.c_str(), stoichiometric_matrix);

    vector<vector<int> > edge_label_graph;
    GraphUtil::init_graph_to_zeroes<int>(edge_label_graph, graph.size());

    Edge edge;
    for (unsigned int i = 0; i < stoichiometric_matrix[0].size(); ++i) {
        find_edge_for_stoichiometric_column(edge, stoichiometric_matrix, i);
        edge_label_graph[edge.u][edge.v] = i;
    }

    vector<vector<unsigned char> > flow_matrix;
    GraphUtil::init_graph_to_zeroes<unsigned char>(flow_matrix, stoichiometric_matrix[0].size(), flux_set.length());

    //each flux is represented by a column in the flow matrix.  foreach column in the flow matrix
    //set 1's in the rows that correspond to edges that are used in the flow.  flux 1 -> 5 -> 3
    //has edges 
    //           u,v = 1,5
    //           u,v = 5,3
    //           u,v = 3,1 (if its an internal cycle and not a chain)
    //
    //any edge such as 1,5 has a unique id stored in the edge_label_graph at row = u, col = v
    //walk through each flux and set 1's in the flux's corresponding column in the flow matrix.
    //look up the row index for active edges in the edge_label_graph[u][v].
    for (int i = 0; i < flux_set.length(); ++i) {
        for (int j = 0; j < flux_set[i].length(); ++j) {
            edge.u = flux_set[i][j];
            edge.v = flux_set[i][(j + 1) % flux_set[i].length()];
            flow_matrix[edge_label_graph[edge.u][edge.v]][i] = 1;
        }
    }

    unsigned int flux_histogram_pointers[flux_set.length()];
    for (unsigned int i = 0; i < flux_set.length(); ++i) {
        flux_histogram_pointers[i] = i;
    }

    sort_flux_histogram_pointers(&flux_histogram_pointers[0], flux_histogram, 0, flux_set.length() - 1);
    order_output_by_flux_histogram_pointers(&flux_histogram_pointers[0], flux_histogram, flow_matrix);

    std::string flow_matrix_file = output_directory + "/flow.matrix";
    ofstream flow_matrix_filestream(flow_matrix_file.c_str());
    write_graph(flow_matrix, flow_matrix_filestream);
    flow_matrix_filestream.close();

    ofstream flux_histogram_file(coefficients_file.c_str());
    for (unsigned int i = 0; i < flux_set.length(); ++i) {
        flux_histogram_file << flux_histogram[i] << std::endl;
    }

    flux_histogram_file.close();
}

void PathFactoryEcoNet::order_output_by_flux_histogram_pointers(unsigned int* flux_histogram_pointers, 
                                                                FluxHistogram& flux_histogram, 
                                                                vector<vector<unsigned char> >& flow_matrix) {
    double tmp_flux_histogram[flow_matrix[0].size()];
    for (unsigned int i = 0; i < flow_matrix[0].size(); ++i) {
        tmp_flux_histogram[i] = flux_histogram[i];
    }
    
    for (unsigned int i = 0; i < flow_matrix[0].size(); ++i) {
        flux_histogram[i] = tmp_flux_histogram[flux_histogram_pointers[i]];
    }

    vector<vector<unsigned char> > tmp_flow_matrix;
    GraphUtil::copy_graph<unsigned char>(flow_matrix, tmp_flow_matrix);

    for (unsigned int i = 0; i < flow_matrix[0].size(); ++i) {
        for (unsigned int j = 0; j < flow_matrix.size(); ++j) {
            flow_matrix[j][i] = tmp_flow_matrix[j][flux_histogram_pointers[i]];
        }
    }
}

void PathFactoryEcoNet::sort_flux_histogram_pointers(unsigned int* flux_histogram_pointers, 
                                                     FluxHistogram& flux_histogram, int l, int r) {
    int i = l, j = r;
    unsigned int tmp;
    double pivot = flux_histogram[flux_histogram_pointers[(l + r) / 2]];

    while (i <= j) {
        while (flux_histogram[flux_histogram_pointers[i]] > pivot) {
            i++;
        }

        while (flux_histogram[flux_histogram_pointers[j]] < pivot) {
            j--;
        }

        if (i <= j) {
            tmp = flux_histogram_pointers[i];
            flux_histogram_pointers[i] = flux_histogram_pointers[j];
            flux_histogram_pointers[j] = tmp;
            i++;
            j--;
        }
    }

    if (l < j) {
        sort_flux_histogram_pointers(flux_histogram_pointers, flux_histogram, l, j);
    }

    if (i < r) {
        sort_flux_histogram_pointers(flux_histogram_pointers, flux_histogram, i, r);
    }
}

void PathFactoryEcoNet::write_graph(vector<vector<unsigned char> >& graph, std::ostream& out) {
    for (unsigned int i = 0; i < graph.size(); ++i) {
        for (unsigned int j = 0; j < graph[i].size(); ++j) {
            out << (int)graph[i][j] << " ";
        }
        
        out << endl;
    }
}

} //namespace nfa
