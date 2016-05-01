#define BOOST_TEST_MODULE Graph
#include <boost/test/included/unit_test.hpp>

#include "Graph.h"

using namespace gis;

BOOST_AUTO_TEST_SUITE(Transposition)

BOOST_AUTO_TEST_CASE(Simple)
{
    Graph g;
    g.vertices.emplace_back("v1");
    g.vertices.emplace_back("v2");

    // v1 has a directed edge to v2
    g.vertices[0].neighbours.emplace_back(g.vertices[1]);

    Graph t = g.transposedGraph();
    BOOST_TEST(t.vertices.size() == 2);
    BOOST_TEST(t.vertices[0].label == g.vertices[0].label);
    BOOST_TEST(t.vertices[1].label == g.vertices[1].label);

    // now v2 should has an edge to v1
    BOOST_TEST(t.vertices[0].neighbours.size() == 0);
    BOOST_TEST(t.vertices[1].neighbours.size() == 1);
    // check if ref is correct
    BOOST_TEST(&(t.vertices[1].neighbours[0].get()) == &(t.vertices[0]));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(FindStronglyConnectedComponents)

BOOST_AUTO_TEST_CASE(Simple)
{
    Graph g;
    g.vertices.emplace_back("v1");
    g.vertices.emplace_back("v2");
    g.vertices.emplace_back("v3");

    // simple cycle between v1, v2 and v3
    g.vertices[0].neighbours.emplace_back(g.vertices[1]);
    g.vertices[1].neighbours.emplace_back(g.vertices[2]);
    g.vertices[2].neighbours.emplace_back(g.vertices[0]);

    const auto scc = g.findStronglyConntectedComponents();
    BOOST_TEST(scc.size() == 1);
    BOOST_TEST(scc.front().size() == 3);
}

BOOST_AUTO_TEST_CASE(SingleUnidirectedEdge)
{
    Graph g;
    g.vertices.emplace_back("v1");
    g.vertices.emplace_back("v2");

    // v1 has a dircted edge to v2
    g.vertices[0].neighbours.emplace_back(g.vertices[1]);

    const auto scc = g.findStronglyConntectedComponents();
    BOOST_TEST(scc.size() == 2);
    BOOST_TEST(scc.front().size() == 1);
    BOOST_TEST(scc.back().size() == 1);
    BOOST_TEST(scc.front().front().get().label != scc.back().front().get().label);
}

BOOST_AUTO_TEST_SUITE_END()
