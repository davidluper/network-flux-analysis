#ifndef RUN_TIME_CONTEXT_H
#define RUN_TIME_CONTEXT_H

#include <string>
#include "FluxSet.h"
#include "GraphUtil.h"
#include "FluxHistogram.h"

namespace nfa {

class PathFactory;

class RuntimeContext
{

friend class StructuralDecomposer;
friend class FunctionalDecomposer;
friend class AdjacencyMatrixBuilder;
friend class DecompositionPlanExecutor;

public:
/*
 * this enun defines the different path input sources.  right now eco net is the only
 * input source defined
 */
enum PathFactoryType
{
    ECONET
};

    RuntimeContext(PathFactoryType path_factory_type, std::string directory, std::string paths_file_name);

    ~RuntimeContext();

    PathFactory& path_factory();
    void set_path_factory(PathFactoryType path_factory_type);

    std::string directory() const;
    void set_directory(std::string& directory);

    void set_paths_file_name(std::string& paths_file);

    std::string adjacency_matrix_file() const;
    void set_adjacency_matrix_file(std::string& adjacency_matrix_file);

    bool verbose_mode() const;
    void set_verbose_mode(bool val);

    bool do_build_adjacency_matrix() const;
    void set_do_build_adjacency_matrix(bool val);

    std::string paths_file() const;
    std::string cycles_file() const;
    std::string coefficients_file() const;
    std::string unique_paths_file() const;

    unsigned int progress_counter() const;
    void increment_progress_counter();

 private:
    PathFactory* m_path_factory;
    unsigned int m_progress_counter;
    std::string m_directory;
    std::string m_cycles_file;
    std::string m_paths_file;
    std::string m_paths_file_name;
    std::string m_unique_paths_file;
    std::string m_coefficients_file;
    std::string m_adjacency_matrix_file;
    bool m_do_build_adjacency_matrix;
    bool m_verbose_mode;

    FluxSet m_flux_set;
    UnweightedGraph m_graph;
    FluxHistogram m_flux_histogram;
};


inline void RuntimeContext::increment_progress_counter()
{
    ++m_progress_counter;
}

} //namespace nfa

#endif
