#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

#include <assert.h>
#include <iostream>
using namespace std;

namespace nfa {

template <class T>
class LinkedList {
    struct LLNode {
        LLNode(T val) {
            this->m_next = 0x00;
            this->m_val = val;
        }
        
        LLNode* m_next;
        T m_val;
        
        void print() {
            cout << m_val << " ";
        }
    };

public:
    LinkedList(const LinkedList& other) {
        cout << "don't call linked list copy constructor" << endl;
        assert(false);
    }

    LinkedList() {
        m_head = 0x00;
        m_tail = 0x00;
        m_size = 0;
    };
    
    ~LinkedList() {
        clear();
    };
    
    int size() { 
        return m_size; 
    }

    void print() {
        for (LinkedList<T>::iterator it = begin(); it != end(); ++it) {
            (*it)->print();
        }

        cout << endl;
    }

    void clear() {
        LLNode* ptr = m_head;
        LLNode* temp;
        while (ptr != 0x00) {
            temp = ptr;
            ptr = ptr->m_next;
            delete temp;
        }
        
        m_head = 0x00;
        m_tail = 0x00;
        m_size = 0;
    }
    
    void push_back(T val) {
        LLNode* node = new LLNode(val);
        
        if (m_tail == 0x00) {
            m_head = node;
            m_tail = node;
        } else {
            m_tail->m_next = node;
            m_tail = node;
        }
        
        m_size += 1;
    };
    
    void push_front(T val) {
        LLNode* node = new LLNode(val);
        
        if (m_head == 0x00) {
            m_head = node;
            m_tail = node;
        } else {
            node->m_next = m_head;
            m_head = node;
        }
        
        m_size += 1;
    };

    T pop_front() {
        assert(m_head);
        LLNode* node = m_head;
        m_head = m_head->m_next;
        T rtn = node->m_val;
        delete node;
    }

    void delete_elements() {
        LLNode* ptr = m_head;
        
        while (ptr != 0x00) {
            delete ptr->m_val;
            ptr = ptr->m_next;
        }
    }
    
    class iterator {
    private:
        LLNode* m_node;

    public:
        iterator(LLNode* node) {
            m_node = node;
        }
        
        iterator& operator=(const iterator& other) {
            m_node = other.m_node;
            return(*this);
        }
        
        bool operator==(const iterator& other) {
            return m_node == other.m_node;
        }
        
        bool operator!=(const iterator& other) {
            return m_node != other.m_node;
        }
        
        iterator& operator++() {
            if (m_node != 0x00) {
                m_node = m_node->m_next;
            }
            return(*this);
        }
        
        iterator& operator++(int) {
            iterator tmp(*this);
            ++(*this);
            return(tmp);
        }
        
        T& operator*() {
            return m_node->m_val;
        }
        
        T* operator->() {
            return(&*(LinkedList<T>::iterator)*this);
        }
        
    };

    iterator begin() { 
        return iterator(m_head); 
    }

    iterator end() { 
        return iterator(0x00); 
    }

    T& head() const { 
        return m_head->m_val; 
    }

    T& tail() const { 
        return m_tail->m_val; 
    }

private:
    LLNode* m_head;
    LLNode* m_tail;    

    int m_size;
};

} // namespace nfa

#endif
