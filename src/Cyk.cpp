#include "Cyk.h"
#include "collections/LinkedList.h"
#include "PathFactory.h"
#include <limits>

namespace nfa {

#define CALC_OFFSET(_x, _y_offset, _z_offset) ((_z_offset) + (_y_offset) + (_x))
#define END (std::numeric_limits<TABLE_CELL_OBJECT_DATA_TYPE>::max() - 1)

Cyk::Cyk(){

}

void Cyk::initialize(Grammar* grammar){
    m_grammar = grammar;
}

/*
 * interpret a path given a grammar of fluxes, use the cyk algorithm thanks Cocke, Younger, and Kasami!
 */
void Cyk::interpret_path_as_fluxes(Path& path, LinkedList<Path*>& flux_labeled_paths, 
                                   RuntimeContext& context) {
    //as is normal with dynamic programming, build a table of values first
    Cyk::Table* P = compute_parse_table(path);
    //now that we have a table of values, trace through it to reconstruct all valid parse trees for the
    //input path using the defined grammar
    interpret_path_using_parse_table(path, *P, flux_labeled_paths, context);

    delete P;
}

void Cyk::interpret_path_using_parse_table(Path& path, Cyk::Table& P, 
                                           LinkedList<Path*>& flux_labeled_paths, 
                                           RuntimeContext& context) {
    bool is_member_of_language = 
        P[CALC_OFFSET(0, (path.length() - 1) * path.length(), 0)] != 0x00 &&
        P[CALC_OFFSET(0, (path.length() - 1) * path.length(), 0)]->length() > 0;

    assert(is_member_of_language);

    Path* path_as_fluxes = context.path_factory().new_empty_path(path.length());
    flux_labeled_paths.push_back(path_as_fluxes);

    trace_parse_trees(0, path.length() - 1, 0, P, -1, flux_labeled_paths, path.length());
}

/*
 * trace back through a parse table to reconstruct all possible parse trees
 */
void Cyk::trace_parse_trees(unsigned int i, unsigned int j, COMPARTMENT_DATA_TYPE k, Cyk::Table& P, 
                            short current_flux_id, LinkedList<Path*>& flux_labeled_paths, 
                            unsigned int path_length) {
    Cyk::TableCellList** ptr = &P[CALC_OFFSET(i, j * path_length, k * path_length * path_length)];
    Cyk::TableCell* tco_ptr = (*(*ptr)).get_ptr();

    LinkedList<Path*>** input_to_next = new LinkedList<Path*>*[(*ptr)->length()];
    input_to_next[0] = &flux_labeled_paths;
    for (int i = 1; i < (*ptr)->length(); ++i) {
        input_to_next[i] = new LinkedList<Path*>();
        for (LinkedList<Path*>::iterator it = flux_labeled_paths.begin(); it != flux_labeled_paths.end(); ++it) {
            Path* p = new Path(**it);
            input_to_next[i]->push_back(p);
        }
    }

    for (int i = 0; i < (*ptr)->length(); ++i) {
        Cyk::TableCell tco = *tco_ptr;
    
        TABLE_CELL_OBJECT_DATA_TYPE production_left = tco[0][2];
        TABLE_CELL_OBJECT_DATA_TYPE production_right = tco[1][2];
        if (production_left == END) {
            assert(production_right == END);
            for (LinkedList<Path*>::iterator it = input_to_next[i]->begin(); it != input_to_next[i]->end(); ++it) {
                (*it)->push_back(current_flux_id);
            }
        } else {
            short flux_id_right_child = current_flux_id;
            if (k < (*m_grammar).literals_length() && production_right >= (*m_grammar).literals_length()) {
                flux_id_right_child = (*m_grammar).get_flux_id_for_production(production_right);
            }
            
            trace_parse_trees(tco[1][0], tco[1][1], tco[1][2], P, flux_id_right_child, 
                              *input_to_next[i], path_length);
            
            short flux_id_left_child = current_flux_id;
            if (k < (*m_grammar).literals_length() && production_left >= (*m_grammar).literals_length()) {
                flux_id_left_child = (*m_grammar).get_flux_id_for_production(production_left);
            }

            trace_parse_trees(tco[0][0], tco[0][1], tco[0][2], P, flux_id_left_child, *input_to_next[i], path_length);
        }
        
        tco_ptr = tco_ptr->next;
    }

    for (int i = 1; i < (*ptr)->length(); ++i) {
        for (LinkedList<Path*>::iterator it = input_to_next[i]->begin(); it != input_to_next[i]->end(); ++it) {
            flux_labeled_paths.push_back(*it);
        }
    }

    for (int i = 1; i < (*ptr)->length(); ++i) {
        delete input_to_next[i];
    }

    delete[] input_to_next;
}

/*
 * compute the parse table http://en.wikipedia.org/wiki/CYK_algorithm
 */
Cyk::Table* Cyk::compute_parse_table(Path& path) {
    Cyk::Table* rtn_P = new Cyk::Table(path, (*m_grammar).total_number_of_productions());    
    Cyk::Table& P = *rtn_P;
    
    unsigned int path_length_squared = path.length() * path.length();

    Cyk::TableCellList** ptr = &P[0];

    for (int i = 0; i < path.length(); ++i) {
        for (Grammar::iterator_literals it_literals = (*m_grammar).begin_literals(); 
             it_literals != (*m_grammar).end_literals(); ++it_literals) {
            if ((*it_literals).head()->B == path[i]) {
                ptr = &P[CALC_OFFSET(i, 0, (*it_literals).head()->B * path_length_squared)];
                if (*ptr == 0x00) {
                    *ptr = new Cyk::TableCellList();
                }

                (*ptr)->push_front(new Cyk::TableCell(END, END, END, END, END, END));
            }
        } 
    }

    Cyk::TableCellList** ptr_a;
    Cyk::TableCellList** ptr_b;
    Cyk::TableCellList** ptr_c;
    long long 
        substring_split_index_times_path_length, 
        substring_start_plus_substring_split_index_plus_1, 
        substring_length_minus_substring_split_index_minus_1,
        substring_length_minus_substring_split_index_minus_1_times_path_length,
        substring_length_times_path_length,
        it_nonliterals_production_index_times_path_length_squared;
    for (int substring_length = 1; substring_length < path.length(); ++substring_length) {
        substring_length_times_path_length = substring_length * path.length();
        for (int substring_start = 0; substring_start < path.length() - substring_length; ++substring_start) {
            for (int substring_split_index = 0; substring_split_index < substring_length; ++substring_split_index) {
                substring_split_index_times_path_length = substring_split_index * path.length();
                substring_start_plus_substring_split_index_plus_1 = substring_start + substring_split_index + 1;
                substring_length_minus_substring_split_index_minus_1 = (substring_length - substring_split_index - 1);
                substring_length_minus_substring_split_index_minus_1_times_path_length = 
                    substring_length_minus_substring_split_index_minus_1 * path.length();
                
                for (Grammar::iterator_nonliterals it_nonliterals = (*m_grammar).begin_nonliterals(); 
                     it_nonliterals != (*m_grammar).end_nonliterals(); ++it_nonliterals) {
                    it_nonliterals_production_index_times_path_length_squared = 
                        it_nonliterals.get_production_ndx() * path_length_squared;
                    
                    for (LinkedList<ProductionRightHandSide*>::iterator it = (*it_nonliterals).begin(); it != (*it_nonliterals).end(); ++it) {
                        ptr_a = &P[
                            CALC_OFFSET(
                                substring_start, 
                                substring_split_index_times_path_length,
                                (*it)->B * path_length_squared
                                )
                            ];

                        ptr_b = &P[
                            CALC_OFFSET(
                                substring_start_plus_substring_split_index_plus_1, 
                                substring_length_minus_substring_split_index_minus_1_times_path_length,
                                (*it)->C * path_length_squared
                                )
                            ];

                        if (*ptr_a != 0x00 && (*ptr_a)->length() > 0 && *ptr_b != 0x00 && (*ptr_b)->length() > 0) {
                            ptr_c = &P[
                                CALC_OFFSET(
                                    substring_start, 
                                    substring_length_times_path_length, 
                                    it_nonliterals_production_index_times_path_length_squared
                                    )
                                ];

                            if (*ptr_c == 0x00) {
                                *ptr_c = new Cyk::TableCellList();
                            }
                            
                            (*ptr_c)->push_front(
                                new Cyk::TableCell(
                                    substring_start, 
                                    substring_split_index, 
                                    (*it)->B, 
                                    substring_start_plus_substring_split_index_plus_1, 
                                    substring_length_minus_substring_split_index_minus_1,
                                    (*it)->C));
                        }
                    }
                }
            }
        }
    }
    
    return rtn_P;
}
  
} //namespace nfa
