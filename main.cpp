#include "Graph.h"

#include <iostream>

void printErrorMsg(const std::ios_base::failure &fail)
{
    std::cerr << "Error while processing input (" << fail.code() << "): " << fail.what() << std::endl;
}

int main()
{
    std::cin.sync_with_stdio(false);
    std::cin.exceptions(std::istream::badbit | std::istream::failbit | std::istream::eofbit);
    try {
        while(true) {
            size_t verticesCount;
            std::cin >> verticesCount;
            gis::Graph g;
            g.vertices.reserve(verticesCount);
            unsigned id = 1;
            std::generate_n(std::back_inserter(g.vertices), verticesCount, [&id](){
               return gis::Vertex(std::to_string(id++));
            });
            try {
                size_t edgesCount;
                std::cin >> edgesCount;
                for (size_t i = 0; i < edgesCount; ++i) {
                    size_t from, to;
                    std::cin >> from >> to;
                    if (from > verticesCount || to > verticesCount || from < 1 || to < 1) {
                        throw std::ios_base::failure("Vertex index out of bounds");
                    }
                    g.vertices[(from-1)].neighbours.emplace_back(g.vertices[(to-1)]);
                }
                const auto scc = g.findStronglyConntectedComponents();
                for (const auto &comp : scc) {
                    std::cout << "{";
                    for (const gis::Vertex &v : comp) {
                        std::cout << v.label << ",";
                    }
                    std::cout << "}, ";
                }
                std::cout << std::endl;
            } catch (const std::ios_base::failure &fail) {
                // TODO: actually exception message is very poor error message
                printErrorMsg(fail);
                break;
            }
        }
    } catch (const std::ios_base::failure &fail) {
        if (!std::cin.eof()) {
            printErrorMsg(fail);
        }
    }
}

