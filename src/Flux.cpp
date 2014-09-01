#include "Flux.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Util.h"
#include <stdio.h>
#include <sstream>
#include <string.h>

namespace nfa {

Flux::Flux() {
    m_compartments = 0x00;
    init(-1, 0);
}

void Flux::init(int flux_id, unsigned int length) {
    if (m_compartments != 0x00) { 
        delete m_compartments; 
    }
    
    m_length = length;
    m_flux_id = flux_id;

    if (m_length > 0) {
        m_total_length = m_length + EXTRA_LENGTH;
        m_compartments = new COMPARTMENT_DATA_TYPE[m_total_length];
    } else {
        m_total_length = 0;
        m_compartments = 0x00;
    }
}

Flux::~Flux() { 
    delete[] m_compartments; 
}

std::string Flux::to_string() {
    std::stringstream ss;
    for (int i = 0; i < m_length; i++) {
        ss << m_compartments[i] << " ";
    }

    return ss.str();
}

/*
 * count the number of tokens in a string
 */
unsigned int Flux::get_number_of_compartments_in_flux(char* s) {
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

/*
 * this loads a flux from a char string of numeric characters seperated
 * by spaces
 */
void Flux::load_flux(char* s) {
    char* ptr = strtok(s, " ");
    int j = 0;
    COMPARTMENT_DATA_TYPE temp;
    while (ptr != 0x00) {
        assert(Util::is_numeric(ptr));
        sscanf(ptr, "%hi", &temp);
        m_compartments[j] = temp;
        
        ptr = strtok(0x00, " ");
        j += 1;
    }
}

int Flux::flux_id() const {
    return m_flux_id;
}

void Flux::set_flux_id(int flux_id) {
    m_flux_id = flux_id;
}

bool Flux::is_chain() {
    return m_compartments[0] == 0 ? true : false;
}

void Flux::set_last_compartment(COMPARTMENT_DATA_TYPE val) {
    m_compartments[m_length] = val;
    m_length += 1;
    
    assert(m_length <= m_total_length);
}
    
void Flux::write_to_stream(ofstream& f) {
    char buf[32];
    for (int i = 0; i < m_length; i++) {
        sprintf(buf, "%d", (int)m_compartments[i]);
        f << buf;
        if (i < m_length - 1) { f << "_"; }
    }
}
    
Flux* Flux::copy() {
    Flux* rtn = new Flux();
    rtn->init(this->m_flux_id, this->m_length);
    for (int i = 0; i < this->m_length; i++) {
        rtn->m_compartments[i] = this->m_compartments[i];
    }
    
    return rtn;
}
    
void Flux::carousel_op() {
    COMPARTMENT_DATA_TYPE temp = m_compartments[0];
    for (int i = 0; i < m_length - 1; i++) {
        m_compartments[i] = m_compartments[(i + 1) % m_length];
    }
    
    m_compartments[m_length - 1] = temp;
}
        
unsigned int Flux::length() const {
    return m_length;
}

void Flux::set_length(unsigned int length) {
    m_length = length;
}
    
} //namespace nfa
