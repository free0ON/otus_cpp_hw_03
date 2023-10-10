/**
 * Custom List
 */

#ifndef ALLOCATOR_TLIST_H
#define ALLOCATOR_TLIST_H

#include <memory>
#include <iostream>

template<typename T>
class TListNode {
public:
    typedef T value_type;
    TListNode* next;
    T* value;

    TListNode()
    {
        value = 0;
    }
    TListNode(const T* value)
    {
        this->value = value;
    }

    TListNode(T& value)
    {
        this->value = &value;
    }

    ~TListNode() {
        delete value;
        delete next;
    }
};

template<typename T>
class TListIterator {
public:
    typedef TListNode<T> NodeType;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
    typedef std::forward_iterator_tag iterator_category;
    NodeType * CurrentNode;

public:
    TListIterator(NodeType* node): CurrentNode(node) {};

    TListIterator(): CurrentNode() {};

    reference operator*()
    {
        return *CurrentNode->value;
    }

    pointer operator->()
    {
        return CurrentNode->value;
    }

    void operator++()
    {
        if(CurrentNode)
            CurrentNode = CurrentNode -> next;
    }

    TListIterator<T> operator++(int)
    {
        TListIterator<T> tmp = *this;
        CurrentNode = CurrentNode -> next;
        return tmp;
    }

    friend bool operator==(const TListIterator & left, const TListIterator & right)
    {
        return left.CurrentNode == right.CurrentNode;
    }

    friend bool operator!=(const TListIterator & left, const TListIterator & right)
    {
        return left.CurrentNode != right.CurrentNode;
    }
};

template<typename T, typename Alloc = std::allocator<T>>
class TList{
public:
    using node_type = TListNode<T>;
    using node_pointer = TListNode<T> *;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = TListIterator<T>;
    typedef typename std::allocator<T>::template rebind<node_type>::other node_allocator;

    TList()
    {
    };

private:
    size_t size = 0;
    node_pointer head {nullptr};
    node_pointer last {nullptr};
    Alloc alloc;
    node_allocator NodeAlloc;

public:
    ~TList() = default;
    TList(TList&&) = default;
    TList& operator=(const TList&) {};
    TList& operator=(TList&&) = delete;

    void push_back(const T& value)
    {
        node_pointer new_node = NodeAlloc.allocate(1);
        new_node -> value = alloc.allocate(1);
        alloc.construct(new_node -> value, value);
        if(size == 0)
            head = new_node;
        else
            last -> next = new_node;
        last = new_node;
        ++size;
    }

    iterator begin()
    {
        return iterator(head);
    }
    iterator end()
    {
        return iterator(last -> next);
    }

};
#endif //ALLOCATOR_TLIST_H