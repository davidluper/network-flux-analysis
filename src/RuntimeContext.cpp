#include "RuntimeContext.h"
#include "PathFactory.h"
#include "PathFactoryEcoNet.h"

namespace nfa {

RuntimeContext::RuntimeContext(PathFactoryType path_factory_type, std::string directory, std::string paths_file_name) {
    m_progress_counter = 0;
    m_do_build_adjacency_matrix = true;
    m_paths_file_name = paths_file_name;
    m_verbose_mode = false;

    set_directory(directory);
    m_path_factory = 0x00;
    set_path_factory(path_factory_type);
}

void RuntimeContext::set_path_factory(PathFactoryType path_factory_type) {
    if (m_path_factory != 0x00) {
        delete m_path_factory;
    }

    switch (path_factory_type)
    {
    case ECONET:
        m_path_factory = new PathFactoryEcoNet();
        break;
    default:
        throw "invalid path factory";
    }
}

RuntimeContext::~RuntimeContext() {
    delete m_path_factory;
}

bool RuntimeContext::verbose_mode() const {
    return m_verbose_mode;
}

void RuntimeContext::set_verbose_mode(bool val) {
    m_verbose_mode = val;
}

bool RuntimeContext::do_build_adjacency_matrix() const {
    return m_do_build_adjacency_matrix;
}

void RuntimeContext:: set_do_build_adjacency_matrix(bool val) {
    m_do_build_adjacency_matrix = val;
}

unsigned int RuntimeContext::progress_counter() const {
    return m_progress_counter;
}

std::string RuntimeContext::coefficients_file() const {
    return m_coefficients_file;
}

std::string RuntimeContext::adjacency_matrix_file() const {
    return m_adjacency_matrix_file;
}

void RuntimeContext::set_adjacency_matrix_file(std::string& adjacency_matrix_file) {
    m_adjacency_matrix_file = m_directory + "/" + adjacency_matrix_file;
}

std::string RuntimeContext::directory() const {
    return m_directory;
}

void RuntimeContext::set_directory(std::string& directory) {
    m_directory = directory;

    m_paths_file = m_directory + "/" + m_paths_file_name;    
    m_cycles_file = m_directory + "/cycles.out";
    m_unique_paths_file = m_directory + "/unique_paths.dat";
    m_coefficients_file = m_directory + "/coefficients.vector";
    m_adjacency_matrix_file = m_directory + "/adjacency.matrix";
}

std::string RuntimeContext::cycles_file() const {
    return m_cycles_file;
}
    
std::string RuntimeContext::paths_file() const {
    return m_paths_file;
}

void RuntimeContext::set_paths_file_name(std::string& paths_file_name) {
    m_paths_file_name = paths_file_name;
    m_paths_file = m_directory + "/" + m_paths_file_name;
}
    
std::string RuntimeContext::unique_paths_file() const {
    return m_unique_paths_file;
}

PathFactory& RuntimeContext::path_factory(){
    return *m_path_factory;
}

}
