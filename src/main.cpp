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

void testAppendLinkedList( std::size_t number_of_elements )
{
    aisdi::LinkedList<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.append(i);
}

void testAppendVector( std::size_t number_of_elements )
{
    aisdi::Vector<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.append(i);
}

void testPrependLinkedList( std::size_t number_of_elements )
{
    aisdi::LinkedList<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.prepend(i);
}

void testPrependVector( std::size_t number_of_elements )
{
    aisdi::Vector<int> collection;
    for( std::size_t i = 0; i < number_of_elements; ++i )
        collection.prepend(i);
}

int main(int argc, char** argv)
{
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 100000;

    /// TEST#1 ===========================================================================
    std::cout << "Test#1: adding element to the end of the collection\n";

    auto start = get_time::now();

    testAppendLinkedList( repeatCount ); /// TEST

    auto diff = get_time::now() - start;

    std::cout << "LinkedList    : " << std::chrono::duration_cast<ns>(diff).count() << " ns\n";

    start = get_time::now();

    testAppendVector( repeatCount ); /// TEST

    auto diff2 = get_time::now() - start;

    std::cout << "Vector        : " << std::chrono::duration_cast<ns>(diff2).count() << " ns\n";

    std::cout << "Difference    : " << std::chrono::duration_cast<ns>(diff-diff2).count() << " ns\n";

    /// TEST#2 ===========================================================================
    std::cout << "Test#2: adding element to the beginning of the collection\n";

    start = get_time::now();

    testPrependLinkedList( repeatCount ); /// TEST

    diff = get_time::now() - start;

    std::cout << "LinkedList    : " << std::chrono::duration_cast<ns>(diff).count() << " ns\n";

    start = get_time::now();

    testPrependVector( repeatCount ); /// TEST

    diff2 = get_time::now() - start;

    std::cout << "Vector        : " << std::chrono::duration_cast<ns>(diff2).count() << " ns\n";

    std::cout << "Difference    : " << std::chrono::duration_cast<ns>(diff-diff2).count() << " ns\n";

    return 0;
}
