#include "Graph.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <chrono>
#include <numeric>
#include "GraphGenerator.h"
#include "Util.h"
#include "MemoryHooks.h"

namespace po = boost::program_options;

/**
 * Simple print to std::cout
 */
void printResult(const std::vector<gis::Vertex::VerticesRefsVector>& scc, std::ostream& os = std::cout) {
    for (const auto &comp : scc) {
        os << "{";
        for (const gis::Vertex &v : comp) {
            os << v.label << ",";
        }
        os << "}, ";
    }
    os << std::endl;
}

/**
 * Function responsible for calling Kosaraju's algorithm implmenetation.
 * Measures time of execution, stack and heap memory used.
 */
util::SingleResult solve(gis::Graph& g, bool verbose, std::ostream& os = std::cout) {
    const auto h1 = memory_measure::getCurrentStackSize();
    const auto t1 = std::chrono::high_resolution_clock::now();
    memory_measure::gEnableHeapMemoryMeasurement = true;
    const auto scc = g.findStronglyConntectedComponents();
    memory_measure::gEnableHeapMemoryMeasurement = false;
    const auto t2 = std::chrono::high_resolution_clock::now();
    const auto heapSize = memory_measure::gMaximumUsedHeapMemory;
    const auto stackSizeDiff = memory_measure::gMaximumStackSize - h1;
    memory_measure::resetHeapMemoryMeasurements();

    if (verbose) {
        printResult(scc, os);
    }

    return util::SingleResult(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count(),
                              stackSizeDiff + heapSize,
                              std::accumulate(g.vertices.begin(), g.vertices.end(), 0u,
                                              [](unsigned long a, const gis::Vertex& b) {
                                                  return a + b.neighbours.size();}));
}

int main(int ac, char* av[]) {
    try {
        po::options_description commandLineOptions("Strongly connected component finder");
        commandLineOptions.add_options()
                ("help", "produce help message")
                ("stdin", "read graph from standard input "
                        "<vertices_count> "
                        "<edges_count> "
                        "<from_id> <to_id> #<edges_count> times")
                ("numberOfTests,t", po::value<unsigned>()->default_value(10),
                 "number of tests with random generator to perform")
                ("numberOfVertices,n", po::value<unsigned>()->default_value(10),
                "number of vertices in generated graph")
                ("probability,p", po::value<double>()->default_value(0.5),
                "probability of generating particular edge")
                ("verbose,v", "enables verbose mode")
                ("autoTest", "generate graphs of given size and run algorithm")
                ("resultFile", po::value<std::string>(), "name of file where results will be saved");

        po::variables_map variablesMap;
        po::store(po::command_line_parser(ac, av).options(commandLineOptions).run(), variablesMap);
        po::notify(variablesMap);

        if (variablesMap.count("help")) {
            std::cout << commandLineOptions << "\n";
            return 0;
        }

        if (variablesMap.count("stdin")) {
            auto g = generator::generateFromStream();
            solve(g, variablesMap.count("verbose") > 0);

        } else if (variablesMap.count("autoTest")) {
            auto tc = variablesMap["numberOfTests"].as<unsigned>();
            auto n = variablesMap["numberOfVertices"].as<unsigned>();
            auto p = variablesMap["probability"].as<double>();
            std::vector<util::SingleResult> results;
            results.reserve(tc);

            while (tc--) {
                auto g = generator::generateRandom(n, p);
                results.push_back(solve(g, variablesMap.count("verbose") > 0));
            }

            if (variablesMap.count("resultFile")) {
                util::writeResultsToCSV(results, variablesMap["resultFile"].as<std::string>());
            }

            auto mean = util::computeMeanValue(results);
            std::cout << mean.duration << " " << mean.stackDiff << " " << mean.edges << std::endl;
        }

    } catch (const std::exception& ex) {
        std::cerr << "Error while processing input :" << ex.what() << std::endl;
    }
}

