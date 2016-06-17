#include "FunctionalDecomposer.h"
#include "PathFactory.h"
#include "FluxHistogram.h"
#include "Path.h"
#include "collections/LinkedList.h"
#include "InterpretationListReducer.h"
#include <iomanip>
#include <iostream>
#include <assert.h>

namespace nfa {

FunctionalDecomposer::FunctionalDecomposer(RuntimeContext& context) :
    m_context(context)
{

}

void FunctionalDecomposer::initialize_decomposition()
{
    //make the flux set
    m_context.m_flux_set.load_fluxes(m_context.cycles_file());
    //----------------------------------------------------------------------

    //create the grammar used in the cyk logic
    m_context.m_flux_set.create_grammar(m_context.m_graph, m_grammar);
    //m_grammar.save_grammar();
    //----------------------------------------------------------------------

    m_cyk.initialize(&m_grammar);
}

void FunctionalDecomposer::decompose()
{
    initialize_decomposition();

    m_context.m_flux_histogram.initialize(&m_context.m_flux_set);

    interpret_paths();
}

void FunctionalDecomposer::interpret_paths()
{
    ifstream f (m_context.unique_paths_file().c_str());

    if (!f)
    {
        cout << "input paths file does not exist: " << m_context.unique_paths_file() << endl << flush;
        assert(false);
    }

    int buf_len = 16384;
    char buf[buf_len];
    char* ptr;

    FluxHistogram path_histogram(&m_context.m_flux_set);

    while (!f.eof())
    {
        m_context.increment_progress_counter();

        f.getline(buf, buf_len);
        stringstream ss(buf);
        int path_multiplier;
        ss >> path_multiplier;

        f.getline(buf, buf_len);
        if (strlen(buf) == 0) {
            continue;
        }
        
        Path* path = m_context.path_factory().new_path_from_internal_string(buf);
        // this is a hard coded stop.  need to work this in as a setting
        if (path->length() > 27) { 
            delete path; 
            continue; 
        }

        LinkedList<Path*> flux_labeled_paths;
        m_cyk.interpret_path_as_fluxes(*path, flux_labeled_paths, m_context);

        LinkedList<Path*> reduced_paths;
        InterpretationListReducer list_reducer(flux_labeled_paths);
        list_reducer.reduce(reduced_paths);

        if (m_context.verbose_mode())
        {
            for (nfa::LinkedList<nfa::Path*>::iterator it = reduced_paths.begin(); it != reduced_paths.end(); ++it)
            {
                cout << "path out$ " << setfill('0') << setw(3) << m_context.progress_counter() << "$ " << (*it)->to_string() << "$" << endl;
            }
        }

        path_histogram.from_labeled_paths(reduced_paths);
        
        path_histogram *= path_multiplier;
        m_context.m_flux_histogram += path_histogram;

        delete path;
        flux_labeled_paths.delete_elements();
    }

    f.close();
}

} //namespace nfa
