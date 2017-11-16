#include <cstddef>
#include <cstdlib>
#include <string>

// For calculating elapsed time
#include <chrono>

#include <iostream>

#include "Vector.h"
#include "LinkedList.h"

using ns = std::chrono::nanoseconds;
using get_time = std::chrono::steady_clock;

ns testAppendLinkedList( std::size_t number_of_elements )
{
    auto start = get_time::now();

    aisdi::LinkedList<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.append(i);

    return std::chrono::duration_cast<ns>(get_time::now() - start);
}

ns testAppendVector( std::size_t number_of_elements )
{
    auto start = get_time::now();

    aisdi::Vector<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.append(i);

    return std::chrono::duration_cast<ns>(get_time::now() - start);
}

ns testPrependLinkedList( std::size_t number_of_elements )
{
    auto start = get_time::now();

    aisdi::LinkedList<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.prepend(i);

    return std::chrono::duration_cast<ns>(get_time::now() - start);
}

ns testPrependVector( std::size_t number_of_elements )
{
    auto start = get_time::now();

    aisdi::Vector<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.prepend(i);

    return std::chrono::duration_cast<ns>(get_time::now() - start);
}

ns testRandomAccessVector( std::size_t number_of_elements )
{
    aisdi::Vector<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.append(i);

    srand( 0 );
    auto start = get_time::now();

    for( std::size_t i = 0; i < number_of_elements; ++i )
        *(collection.begin()+(rand()%number_of_elements));

    return std::chrono::duration_cast<ns>(get_time::now() - start);

}

ns testRandomAccessLinkedList( std::size_t number_of_elements )
{
    aisdi::LinkedList<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.append(i);

    srand( 0 );
    auto start = get_time::now();

    for( std::size_t i = 0; i < number_of_elements; ++i )
        *(collection.begin()+(rand()%number_of_elements));

    return std::chrono::duration_cast<ns>(get_time::now() - start);
}


int main(int argc, char** argv)
{
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 100000;

    /// TEST#1 ===========================================================================
    std::cout << "Test#1: adding element to the end of the collection\n";

    auto diff = testAppendLinkedList( repeatCount );

    std::cout << "LinkedList    : " << std::chrono::duration_cast<ns>(diff).count() << " ns\n";

    auto diff2 = testAppendVector( repeatCount );

    std::cout << "Vector        : " << std::chrono::duration_cast<ns>(diff2).count() << " ns\n";

    std::cout << "Difference    : " << std::chrono::duration_cast<ns>(diff-diff2).count() << " ns\n\n";

    /// TEST#2 ===========================================================================
    std::cout << "Test#2: adding element to the beginning of the collection\n";

    diff = testPrependLinkedList( repeatCount );

    std::cout << "LinkedList    : " << std::chrono::duration_cast<ns>(diff).count() << " ns\n";

    diff2 = testPrependVector( repeatCount );

    std::cout << "Vector        : " << std::chrono::duration_cast<ns>(diff2).count() << " ns\n";

    std::cout << "Difference    : " << std::chrono::duration_cast<ns>(diff-diff2).count() << " ns\n\n";

    /// TEST#3 ===========================================================================
    std::cout << "Test#3: random access\n";

    diff = testRandomAccessLinkedList( repeatCount );

    std::cout << "LinkedList    : " << std::chrono::duration_cast<ns>(diff).count() << " ns\n";

    diff2 = testRandomAccessVector( repeatCount );

    std::cout << "Vector        : " << std::chrono::duration_cast<ns>(diff2).count() << " ns\n";

    std::cout << "Difference    : " << std::chrono::duration_cast<ns>(diff-diff2).count() << " ns\n\n";

    return 0;
}
