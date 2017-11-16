#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

//#include <iostream>

namespace aisdi
{
    template <typename Type>
    class LinkedList
    {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        LinkedList()
        {
            head = new Node;
            tail = new Node;

            head->next = tail;
            head->prev = nullptr;
            tail->next = nullptr;
            tail->prev = head;

            size_of_container = 0;
        }

        LinkedList(std::initializer_list<Type> l) : LinkedList()
        {
            /*head = new Node;
            tail = new Node;

            head->next = tail;
            head->prev = nullptr;
            tail->next = nullptr;
            tail->prev = head;

            size_of_container = 0;*/

            for( auto it = l.begin(); it != l.end(); ++it )
                append(*it);
        }

        LinkedList(const LinkedList& other) : LinkedList()
        {
            /*head = new Node;
            tail = new Node;

            head->next = tail;
            head->prev = nullptr;
            tail->next = nullptr;
            tail->prev = head;

            size_of_container = 0;*/

            for( auto it = other.begin(); it != other.end(); ++it )
                append(*it);
        }

        LinkedList(LinkedList&& other)
        {
            head = other.head;
            tail = other.tail;
            size_of_container = other.size_of_container;

            other.head = new Node;
            other.tail = new Node;

            other.head->next = other.tail;
            other.head->prev = nullptr;
            other.tail->next = nullptr;
            other.tail->prev = other.head;

            other.size_of_container = 0;
        }

        ~LinkedList()
        {
            erase( begin(), end() );
            delete head;
            delete tail;
        }

        LinkedList& operator=(const LinkedList& other)
        {
            if( (*this).head == other.head )
                return *this;
            erase( begin(), end() );

            for( auto it = other.begin(); it != other.end(); ++it )
                append(*it);

            return *this;
        }

        LinkedList& operator=(LinkedList&& other)
        {
            if( (*this).head == other.head )
                return *this;

            erase( begin(), end() );
            delete head;
            delete tail;

            head = other.head;
            tail = other.tail;
            size_of_container = other.size_of_container;

            // We can make a new head and tail if we want to keep 'other' operative
            other.head = new Node;
            other.tail = new Node;

            other.head->next = other.tail;
            other.head->prev = nullptr;
            other.tail->next = nullptr;
            other.tail->prev = other.head;

            other.size_of_container = 0;

            return *this;
        }

        bool isEmpty() const
        {
            return ( head->next == tail ); // Or just ( size_of_container == 0 )
        }

        size_type getSize() const
        {
            return size_of_container;
        }

        void append(const Type& item)
        {
            auto new_node = new Node;

            tail->prev->next = new_node;

            new_node->next = tail;
            new_node->prev = tail->prev;

            tail->prev = new_node;

            new_node->value = new value_type;
            *(new_node->value) = item;

            ++size_of_container;
        }

        void prepend(const Type& item)
        {
            auto new_node = new Node;

            head->next->prev = new_node;

            new_node->prev = head;
            new_node->next = head->next;

            head->next = new_node;

            new_node->value = new value_type;
            *(new_node->value) = item;

            ++size_of_container;
        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if( insertPosition.ptr_to_node == nullptr )
                std::out_of_range("insert(): iterator doesn't point to a anything");

            if( insertPosition.ptr_to_node->next == nullptr ) // so it is tail
            {
                append( item );
            }
            else if( insertPosition.ptr_to_node->prev->prev == nullptr ) // so it is head
            {
                prepend( item );
            }
            else // so it is in the middle
            {
                auto new_node = new Node;
                new_node->value = new value_type;
                *(new_node->value) = item;

                new_node->next = insertPosition.ptr_to_node;
                new_node->prev = insertPosition.ptr_to_node->prev;
                new_node->prev->next = new_node;
                insertPosition.ptr_to_node->prev = new_node;

                ++size_of_container;
            }
        }

        Type popFirst()
        {
            if( isEmpty() )
                throw std::out_of_range("popFirst(): Empty list");

            Node* node = head->next;
            value_type tmp = *(node->value);
            head->next = node->next;
            head->next->prev = head;

            delete node;
            --size_of_container;

            return tmp;
        }

        Type popLast()
        {
            if( isEmpty() )
                throw std::out_of_range("popLast(): Empty list");

            Node* node = tail->prev;
            value_type tmp = *(node->value);
            tail->prev = node->prev;
            tail->prev->next = tail;

            delete node;
            --size_of_container;

            return tmp;
        }

        void erase(const const_iterator& possition)
        {
            if( possition.ptr_to_node == nullptr || possition.ptr_to_node->next == nullptr || possition.ptr_to_node->prev == nullptr )
                throw std::out_of_range("erase(): guardian or empty");

            possition.ptr_to_node->next->prev = possition.ptr_to_node->prev;
            possition.ptr_to_node->prev->next = possition.ptr_to_node->next;
            delete possition.ptr_to_node;

            --size_of_container;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {
            // I can do this more effective but i have no time...
            for( auto it1 = firstIncluded; it1 != lastExcluded; )
            {
                //auto it2 = it1 + 1;
                erase(it1++);
                //it1 = it2;
            }
        }

        iterator begin()
        {
            return iterator(cbegin());
        }

        iterator end()
        {
            return iterator(cend());
        }

        const_iterator cbegin() const
        {
            ConstIterator it;
            it.ptr_to_node = head->next;
            return it;
        }

        const_iterator cend() const
        {
            ConstIterator it;
            it.ptr_to_node = tail;
            return it;
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }

private:

        size_type size_of_container;

        class Node
        {
            public:
            Node(){ value = nullptr; next = nullptr; prev = nullptr; }
            ~Node(){ if( value ) delete value; }
            value_type* value;
            Node* next;
            Node* prev;
        };

        // head and tail acts as a guardians
        Node* head;
        Node* tail;
    };

    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
        friend class LinkedList;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        explicit ConstIterator()
        {
            ptr_to_node = nullptr;
        }

        reference operator*() const
        {
            if( ptr_to_node == nullptr || ptr_to_node->prev == nullptr || ptr_to_node->next == nullptr )
                throw std::out_of_range("dereferencing out of range");

            return *(ptr_to_node->value);
        }

        ConstIterator& operator++()
        {
            if( ptr_to_node->next == nullptr )
                throw std::out_of_range("++iterator: out of range");

            ptr_to_node = ptr_to_node->next;
            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        ConstIterator& operator--()
        {
            if( ptr_to_node->prev->prev == nullptr )
                throw std::out_of_range("++iterator: out of range");

            ptr_to_node = ptr_to_node->prev;
            return *this;
        }

        ConstIterator operator--(int)
        {
            ConstIterator tmp = *this;
            --(*this);
            return tmp;
        }

        ConstIterator operator+(difference_type d) const
        {
            ConstIterator tmp = *this;

            while( d > 0 )
            {
                if( tmp.ptr_to_node->next == nullptr )
                    throw std::out_of_range("operator+: out of range");

                ++tmp;
                --d;
            }

            return tmp;
        }

        ConstIterator operator-(difference_type d) const
        {
            ConstIterator tmp = *this;

            while( d > 0 )
            {
                if( tmp.ptr_to_node->prev == nullptr )
                    throw std::out_of_range("operator-: out of range");

                --tmp;
                --d;
            }

            return tmp;
        }

        bool operator==(const ConstIterator& other) const
        {
            return ( ptr_to_node == other.ptr_to_node );
        }

        bool operator!=(const ConstIterator& other) const
        {
            return ( ptr_to_node != other.ptr_to_node );
        }

    private:
        LinkedList<Type>::Node* ptr_to_node;

    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator()
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
