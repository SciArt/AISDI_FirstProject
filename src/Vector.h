#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
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

    // OK
    Vector()
    {
        first_empty = 0;
        size_of_container = 0;
        array = nullptr;
    }

    // OK
    Vector(std::initializer_list<Type> l)
    {
        first_empty = 0;
        size_of_container = l.size();
        array = new Type[size_of_container];

        for( auto it = l.begin(); it != l.end(); ++it )
            append( *it );
    }

    // OK
    Vector(const Vector& other)
    {
        first_empty = 0;
        size_of_container = other.getSize();
        array = new Type[size_of_container];

        for( auto it = other.begin(); it != other.end(); ++it )
            append( *it );
    }

    // OK
    Vector(Vector&& other)
    {
        // Copying data from 'other'
        first_empty = other.first_empty;
        size_of_container = other.size_of_container;
        array = other.array;

        // Cleaning 'other'
        other.array = nullptr;
        other.first_empty = 0;
        other.size_of_container = 0;
    }

    // OK
    ~Vector()
    {
        // Just delete all data in array
        delete[] array;
    }

    // OK - but it might be worth checking if errors occur
    Vector& operator=(const Vector& other)
    {
        delete[] array;
        array = nullptr;
        first_empty = 0;
        size_of_container = 0;

        for( auto it = other.begin(); it != other.end(); ++it )
                append( *it );

        return *this;
    }

    // OK ?
    Vector& operator=(Vector&& other)
    {
        delete[] array;

        array = other.array;
        first_empty = other.first_empty;
        size_of_container = other.size_of_container;

        other.array = nullptr;
        other.first_empty = 0;
        other.size_of_container = 0;

        return *this;
    }

    // OK
    bool isEmpty() const
    {
        return (first_empty == 0);
    }

    // OK
    size_type getSize() const
    {
        return first_empty;
    }

    // OK
    void append(const Type& item)
    {
        if( size_of_container-first_empty <= 0 )
            allocate_more_space();

        array[first_empty] = item;
    }

    // OK
    void prepend(const Type& item)
    {
       if( size_of_container-first_empty <= 0 )
       {
            allocate_more_space( true );
       }
       else
       {
            // Moving all elements by 1
            for( size_type i = 0; i < first_empty; ++i )
                array[i+1] = array[i];

            ++first_empty;
       }

       array[0] = item;
    }

    // TODO
    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if( size_of_container <= 0 )
            allocate_more_space();

        // TODO i need iterator

        for(size_type i = insertPosition.index_in_array; i < first_empty; ++i)
        {
            array[i+1] = array[i];
        }
        ++first_empty;
        array[insertPosition.index_in_array] = item;
    }

    // OK
    Type popFirst()
    {
        value_type tmp = array[0];

        for( size_type i = 1; i < first_empty; ++i )
            array[i-1] = array[i];

        --first_empty;

        return tmp;
    }

    // OK
    Type popLast()
    {
        return array[--first_empty];
    }

    void erase(const const_iterator& possition)
    {
        for( size_type i = possition.index_in_array; i < first_empty)
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        (void)firstIncluded;
        (void)lastExcluded;
        throw std::runtime_error("TODO");
    }

    iterator begin()
    {
        throw std::runtime_error("TODO");
    }

    iterator end()
    {
        throw std::runtime_error("TODO");
    }

    const_iterator cbegin() const
    {
        throw std::runtime_error("TODO");
    }

    const_iterator cend() const
    {
        throw std::runtime_error("TODO");
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
    value_type* array;

    size_type size_of_container;
    size_type first_empty;

    static const size_type size_of_preallocate = 50;

    void allocate_more_space( bool first_element_empty = false )
    {
        value_type* tmp = new value_type[size_of_container+size_of_preallocate];

        for( size_type i = first_element_empty ? 1 : 0; i < size_of_container; ++i )
            tmp[i] = array[i];

        size_of_container += size_of_preallocate;

        delete[] array;
        array = tmp;
    }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;

    explicit ConstIterator()
    {
        ptr_to_element = nullptr;
        ptr_to_vector = nullptr;
        index_in_array = 0;
    }

    reference operator*() const
    {
        return *ptr_to_element;
    }

    ConstIterator& operator++()
    {
        ++ptr_to_element;
        ++index_in_array;
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
        --ptr_to_element;
        --index_in_array;
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
        tmp.ptr_to_element += d;
        tmp.index_in_array += d;
        return tmp;
    }

    ConstIterator operator-(difference_type d) const
    {
        ConstIterator tmp = *this;
        tmp.ptr_to_element -= d;
        tmp.index_in_array -= d;
        return tmp;
    }

    bool operator==(const ConstIterator& other) const
    {
        return ( ptr_to_vector == other.ptr_to_vector && ptr_to_element == other.ptr_to_element );
    }

    bool operator!=(const ConstIterator& other) const
    {
        return ( ptr_to_vector != other.ptr_to_vector || ptr_to_element != other.ptr_to_element );
    }

private:
    const Vector<Type>* ptr_to_vector;
    pointer ptr_to_element;
    size_type index_in_array;
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

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

#endif // AISDI_LINEAR_VECTOR_H
