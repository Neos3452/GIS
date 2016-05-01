#define BOOST_TEST_MODULE Vertex
#include <boost/test/included/unit_test.hpp>

#include "Vertex.h"

using namespace gis;

BOOST_AUTO_TEST_CASE(CopyWithoutNeighbours)
{
    Vertex first("first");
    first.neighbours.emplace_back(first);
    BOOST_TEST(first.label == "first");

    Vertex second = first.copyWithoutNeighbours();
    BOOST_TEST(first.label == second.label);
    BOOST_TEST(second.neighbours.size() == 0);
}

