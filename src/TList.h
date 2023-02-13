/**
 *
 */

#ifndef ALLOCATOR_TLIST_H
#define ALLOCATOR_TLIST_H

#include <memory>
#include <iterator>
#include "TIterator.h"

template<typename T, typename Alloc = std::allocator<T>, typename Iter = std::iterator<T,std::forward_iterator_tag>>
class TList {
private:
    class Node {
    public:
        ~Node() {
            delete next;
        }
        T value;
        Node* next{ nullptr };
        Node(){
            value = 0;
        }
        Node(const T& value)
        {
            this->value = value;
        }
        T& GetValue(){
            return value;
        }
    };

public:
    using node_type = Node;
    using node_pointer = Node*;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = Iter;
private:
    node_pointer head{ nullptr };
    node_pointer tail{ nullptr };
    Alloc alloc;

public:
    TList() = default;
    ~TList() {
        delete head;
    }
    TList(const TList&) = delete;
    TList(TList&&) = default;
    TList& operator=(const TList& ) {};
    TList& operator=(TList&&) = delete;
//    TList(const Alloc& alloc = Alloc()): alloc(alloc)
//    {
//

    void insert(Iter it, const T& value);

    void push_back(int value) {
        if (tail) {
            tail->next = new node_type;
            tail = tail->next;
        } else {
            head = new node_type;
            tail = head;
        }
        tail->value = value;
    }

    Iter next() {
        return tail -> next;
    }

    Iter begin() {
        return { head };
    }
    Iter end() {
        return { tail };
    }

};
#endif //ALLOCATOR_TLIST_H
