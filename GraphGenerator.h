//
// Created by Przemek Piórkowski on 29.05.2016.
//

#ifndef STRONGLYCONNECTEDCOMPONENTSFINDER_RANDOMGRAPHGENERATOR_H
#define STRONGLYCONNECTEDCOMPONENTSFINDER_RANDOMGRAPHGENERATOR_H

#include <boost/graph/directed_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <memory>

namespace generator {

    using AdjList = boost::adjacency_list<boost::setS, boost::vecS, boost::directedS>;
    using RandomGraphGenerator =  boost::erdos_renyi_iterator<boost::minstd_rand, AdjList>;
    static boost::minstd_rand randomNumber(time(nullptr));

    gis::Graph generateRandom(unsigned n, double p) {
        AdjList adjList(RandomGraphGenerator(randomNumber, n, p), RandomGraphGenerator(), n);
        gis::Graph graph;
        unsigned id = 0;

        graph.vertices.reserve(boost::num_vertices(adjList));
        std::generate_n(std::back_inserter(graph.vertices), boost::num_vertices(adjList), [&id](){
            return gis::Vertex(std::to_string(id++));
        });
        auto edges = boost::edges(adjList);
        for (auto it = edges.first; it != edges.second; ++it) {
            graph.vertices[(*it).m_source].neighbours.emplace_back(graph.vertices[(*it).m_target]);
        }

        return graph;
    }

    gis::Graph generateFromStream(std::istream& is = std::cin) {
        gis::Graph g;
        size_t verticesCount;
        size_t edgesCount;

        is >> verticesCount;
        is >> edgesCount;

        g.vertices.reserve(verticesCount);
        unsigned id = 0;
        std::generate_n(std::back_inserter(g.vertices), verticesCount, [&id](){
            return gis::Vertex(std::to_string(id++));
        });

        for (size_t i = 0; i < edgesCount; ++i) {
            size_t from, to;
            std::cin >> from >> to;
            if (from >= verticesCount || to >= verticesCount ) {
                throw std::ios_base::failure("Vertex index out of bounds");
            }
            g.vertices[from].neighbours.emplace_back(g.vertices[to]);
        }
        return g;
    }
}

#endif //STRONGLYCONNECTEDCOMPONENTSFINDER_RANDOMGRAPHGENERATOR_H
