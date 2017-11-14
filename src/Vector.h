#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include <iostream>

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
        if( array == other.array )
            return *this;

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
        ++first_empty;
    }

    // OK
    void prepend(const Type& item)
    {
        std::cout << "Przed:\n";
        for( auto it = begin(); it != end(); ++it )
            std::cout << (*it) << " ";
        std::cout << std::endl;
        std::cout << "size: " << size_of_container << std::endl;;
        if( size_of_container <= first_empty )
        {
            allocate_more_space( true );
            std::cout << "size: " << size_of_container << std::endl;;
        }
        else
        {
            // Moving all elements by 1
            //for( size_type i = 0; i < first_empty; ++i )
            //    array[i+1] = array[i];

            for( size_type i = first_empty; i > 0; --i )
            {
                std::cout << array[i] << "<-->" << array[i-1] << std::endl;
                array[i] = array[i-1];
            }
        }
        ++first_empty;
        array[0] = item;

        std::cout << "Sprawdzenie:\n";
        for( auto it = begin(); it != end(); ++it )
            std::cout << (*it) << " ";
        std::cout << std::endl;
        std::cout << "Koniec sprawdzenia\n";
    }

    // OK
    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if( insertPosition.index_in_array > first_empty )
            throw std::out_of_range("insert(): there is no such a place in array");

        if( size_of_container <= first_empty )
            allocate_more_space();

        for(size_type i = first_empty; i > insertPosition.index_in_array; --i)
        {
            array[i] = array[i-1];
        }
        ++first_empty;
        array[insertPosition.index_in_array] = item;
    }

    // OK
    Type popFirst()
    {
        if( isEmpty() )
            throw std::logic_error("popFirst when empty collection");
        value_type tmp = array[0];

        for( size_type i = 1; i < first_empty; ++i )
            array[i-1] = array[i];

        --first_empty;

        return tmp;
    }

    // OK
    Type popLast()
    {
        if( isEmpty() )
            throw std::logic_error("popLast when empty collection");
        return array[--first_empty];
    }

    // OK
    void erase(const const_iterator& possition)
    {
        if( possition.index_in_array >= first_empty )
            throw std::out_of_range("erase() - there are no elements to delete!");

        --first_empty;
        for( size_type i = possition.index_in_array; i < first_empty; ++i )
            array[i] = array[i+1];
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        // ZROBIC OUT OF RANGE

        if( firstIncluded == lastExcluded )
            return;

        /// First situation    [#,#,#, , , , , , ]

        if( firstIncluded.index_in_array == lastExcluded.index_in_array-1 )
        {
            erase(firstIncluded);
        }
        else if( firstIncluded == begin() )
        {
            for( size_type i = 0; i < lastExcluded.index_in_array; ++i )
                array[i] = array[i+lastExcluded.index_in_array];
            first_empty -= lastExcluded.index_in_array;
        }

        /// Second situation   [ , , ,#,#,#, , , ]

        else if( firstIncluded.index_in_array > 0 && lastExcluded.index_in_array < first_empty )
        {
            for( size_type i = firstIncluded.index_in_array; i < first_empty; ++i )
                array[i] = array[i+lastExcluded.index_in_array];
            first_empty -= lastExcluded.index_in_array - firstIncluded.index_in_array;
        }

        /// Third situation    [ , , , , , ,#,#,#]

        else if( firstIncluded.index_in_array > 0 && lastExcluded.index_in_array == first_empty)
        {
            first_empty = firstIncluded.index_in_array;
        }
        /// Else... OUT OF RANGE
        else
            throw std::out_of_range("erase() out of range.");


        /*

        if( lastExcluded.index_in_array + 1 > first_empty )
        {
            first_empty = firstIncluded.index_in_array;
        }
        else
        {
            array[firstIncluded.index_in_array+1] = array[lastExcluded.index_in_array+1];
            first_empty = firstIncluded.index_in_array+1;
        }
        */
    }

    iterator begin()
    {
        return const_iterator(cbegin());
    }

    iterator end()
    {
        return const_iterator(cend());
    }

    const_iterator cbegin() const
    {
        const_iterator it;
        it.index_in_array = 0;
        it.ptr_to_element = array;
        it.ptr_to_vector = this;
        return it;
    }

    const_iterator cend() const
    {
        const_iterator it;
        it.index_in_array = first_empty;
        it.ptr_to_element = array+it.index_in_array;
        it.ptr_to_vector = this;
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
    value_type* array;

    size_type size_of_container;
    size_type first_empty;

    static const size_type size_of_preallocate = 50;

    void allocate_more_space( bool first_element_empty = false )
    {
        //(void)first_element_empty;
        value_type* tmp = new value_type[size_of_container+size_of_preallocate];

        for( size_type i = (first_element_empty ? 1 : 0), j = 0; j < size_of_container; ++i, ++j )
            tmp[i] = array[j];

        size_of_container += size_of_preallocate;

        delete[] array;
        array = tmp;
    }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
friend class Vector;
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
        if( (*this) == ptr_to_vector->cend() )
            throw std::out_of_range("Dereferencing out of range.");
        return *ptr_to_element;
    }

    ConstIterator& operator++()
    {
        if( (*this) == ptr_to_vector->cend() )
            throw std::out_of_range("++iterator out of range");

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
        if( (*this) == ptr_to_vector->cbegin() )
            throw std::out_of_range("--iterator out of range");

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
    size_type index_in_array;
    const Vector<Type>* ptr_to_vector;
    pointer ptr_to_element;

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
