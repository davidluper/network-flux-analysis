#include "FluxSet.h"
#include "Grammar.h"
#include "Util.h"

namespace nfa {

FluxSet::FluxSet() {

}

FluxSet::~FluxSet() {
    delete[] m_fluxes;
}

unsigned int FluxSet::length() const {
    return m_length;
}

Flux& FluxSet::operator[] (unsigned int i) { 
    return m_fluxes[i]; 
}

const Flux& FluxSet::operator[] (unsigned int i) const { 
    return m_fluxes[i]; 
}

void FluxSet::create_grammar(UnweightedGraph& graph, Grammar& grammar) {
    grammar.initialize(graph, (*this));
    grammar.build_grammar_rules((*this));
    grammar.transform_to_chomsky(graph);
}

void FluxSet::load_fluxes(const std::string& cycles_file) {
    ifstream f(cycles_file.c_str());
    
    if (!f) {
        cout << "cycles.out file does not exist: " << cycles_file << endl << flush;
    }

    char buf[1024];
    char* ptr;
    COMPARTMENT_DATA_TYPE temp[64];
    int i_temp = 0;
    int temp_cnt = 0;
    int flux_cnt = 0;
    //do a first pass to see how many fluxes there are in the file
    //and also insert the fluxes in to the flux to id indexer
    while (!f.eof()) {
        f.getline(buf, 1023);
        if (buf[0] == 0x00) { 
            continue; 
        }

        temp_cnt = 0;
        
        ptr = strtok(buf, " ");
        while (ptr != 0x00) {
            if (!Util::is_numeric(ptr)) {
                ptr = strtok(0x00, " ");
                continue;
            }
            
            sscanf(ptr, "%d", &i_temp);
            temp[temp_cnt] = (COMPARTMENT_DATA_TYPE)i_temp;
            ptr = strtok(0x00, " ");
            temp_cnt += 1;
        }
        
        flux_cnt += 1;
    }
    
    f.close();
    
    Flux* flux_set_out = new Flux[flux_cnt];
    flux_cnt = 0;
    
    f.open(cycles_file.c_str());
    //second pass stores the fluxes in an array
    while (!f.eof()) {
        f.getline(buf, 1023);
        if (buf[0] == 0x00) { 
            continue; 
        }

        temp_cnt = 0;
        
        ptr = strtok(buf, " ");
        while (ptr != 0x00) {
            if (!Util::is_numeric(ptr)) {
                ptr = strtok(0x00, " ");
                continue;
            }
            
            sscanf(ptr, "%d", &i_temp);
            temp[temp_cnt] = (COMPARTMENT_DATA_TYPE)i_temp;
            ptr = strtok(0x00, " ");
            temp_cnt += 1;
        }
        
        flux_set_out[flux_cnt].init(flux_cnt, temp_cnt);
        for (int i = 0; i < temp_cnt; i++) {
            flux_set_out[flux_cnt][i] = temp[i];
        }
        
        flux_cnt += 1;
    }
    
    f.close();
    
    m_fluxes = flux_set_out;
    m_length = flux_cnt;
}

} //namespace nfa
