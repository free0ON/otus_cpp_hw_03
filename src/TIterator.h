/**
 * Custom Iterator class
 */
#ifndef ALLOCATOR_TITERATOR_H
#define ALLOCATOR_TITERATOR_H

template<typename T>
class TIterator {
private:
    T* CurrentNode;
public:
    TIterator(T* node) : CurrentNode(node) {}
    TIterator& operator++() {
        CurrentNode = CurrentNode -> next;
        return *this;
    }

    T& begin()
    {
        return CurrentNode -> begin();
    }

    T& end ()
    {
        return CurrentNode -> end();
    }

    bool operator< (TIterator other) const { return CurrentNode < other.CurrentNode; }
    bool operator==(TIterator other) const { return CurrentNode == other.CurrentNode; }
    bool operator!=(TIterator other) const { return *this != other; }
    T& operator*() { return CurrentNode->GetValue(); }
};


#endif //ALLOCATOR_TITERRATOR_H
