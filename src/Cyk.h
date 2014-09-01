#ifndef _CYK_H
#define _CYK_H

#include "Grammar.h"
#include "Path.h"
#include <assert.h>
#include <iostream>
#include "RuntimeContext.h"

using namespace std;

namespace nfa {

class Cyk {
public:
    Cyk();
    void initialize(Grammar* grammar);
    void interpret_path_as_fluxes(Path& path, LinkedList<Path*>& flux_labeled_paths, 
                                  RuntimeContext& context);

private:
    Grammar* m_grammar;

#define TABLE_CELL_OBJECT_DATA_TYPE unsigned short

    struct TableCell {
        TableCell(
            TABLE_CELL_OBJECT_DATA_TYPE _0, 
            TABLE_CELL_OBJECT_DATA_TYPE _1, 
            TABLE_CELL_OBJECT_DATA_TYPE _2, 
            TABLE_CELL_OBJECT_DATA_TYPE _3, 
            TABLE_CELL_OBJECT_DATA_TYPE _4, 
            TABLE_CELL_OBJECT_DATA_TYPE _5
            ) {
            m_vals[0][0] = _0;
            m_vals[0][1] = _1;
            m_vals[0][2] = _2;
            
            m_vals[1][0] = _3;
            m_vals[1][1] = _4;
            m_vals[1][2] = _5;
        };

        void print() {
            cout << m_vals[0][0] << " " << m_vals[0][1] << " " << m_vals[0][2] << endl;
            cout << m_vals[1][0] << " " << m_vals[1][1] << " " << m_vals[1][2] << endl;
        }

        TableCell* next;
        TABLE_CELL_OBJECT_DATA_TYPE* operator[](unsigned int ndx) {
            return &m_vals[ndx][0];
        };

    private:
        TABLE_CELL_OBJECT_DATA_TYPE m_vals[2][3];
    };

    struct TableCellList {
        TableCellList() {
            m_length = 0;
            ptr = 0x00;
        }

        ~TableCellList() {
            TableCell* it = ptr;
            TableCell* temp;
            while (it != 0x00) {
                temp = it;
                it = it->next;

                delete temp;
            }
        }

        void push_front(TableCell* item) {
            item->next = ptr;
            ptr = item;
            m_length += 1;
            assert(m_length < 255);
        }
        
        TableCell*  get_ptr() const { 
            return ptr; 
        } 

        void print() {
            TableCell* it = ptr;
            while (it != 0x00) {
                it->print();
                it = it->next;
            }
        }

        unsigned int length() const { 
            return m_length; 
        }

    private:
        unsigned char m_length;
        TableCell* ptr;
    };

    struct Table {
    public:
        Table(Path& path, unsigned int total_number_of_grammar_productions) {
            m_length = path.length() * path.length() * total_number_of_grammar_productions;
            m_P = new TableCellList*[m_length];
            
            TableCellList** ptr = &m_P[0];
            while (ptr <= &m_P[m_length - 1]) {
                *ptr = 0x00;
                ++ptr;
            }
        }

        ~Table() {
            TableCellList** ptr = &m_P[0];
            while (ptr <= &m_P[m_length - 1]) {
                if (*ptr != 0x00) {
                    delete *ptr;
                }
                ++ptr;
            }

            delete[] m_P;
        }

        TableCellList*& operator[](unsigned long long ndx) {
            return m_P[ndx];
        }

    private:
        TableCellList** m_P;
        unsigned int m_length;
    };

    void trace_parse_trees(unsigned int i, unsigned int j, COMPARTMENT_DATA_TYPE k, 
                           Table& P, short flux_id, LinkedList<Path*>& flux_labeled_paths, 
                           unsigned int path_length);
    void interpret_path_using_parse_table(Path& path, Table& P, LinkedList<Path*>& flux_labeled_paths, 
                                          RuntimeContext& context);
    Table* compute_parse_table(Path& path);    

};

} //namespace nfa

#endif
