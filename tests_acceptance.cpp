#define BOOST_TEST_MODULE Acceptance
#include <boost/test/included/unit_test.hpp>

#include "Graph.h"
#include "config.h"
#include "GraphGenerator.h"

using namespace gis;

struct AcceptanceTestsFixture {

    using StringVec = std::set<std::string>;

    const std::string acceptanceTestsBasePath;
    const std::string dagFile;
    const std::string dgFile;
    const std::string fullFile;
    const std::string noEdgesFile;
    const std::string parallelEdgesFile;
    const std::string scgFile;

    AcceptanceTestsFixture() : acceptanceTestsBasePath{acceptanceTestsResourcesDir()},
                               dagFile{"tinyDAG.txt"}, dgFile{"tinyDG.txt"},
                               fullFile{"tinyFull.txt"}, noEdgesFile{"tinyNoEdges.txt"},
                               parallelEdgesFile{"tinyParallelEdges.txt"}, scgFile{"tinySCG.txt"} { }

    Graph readGraph(const std::string& fileName) {
        std::ifstream file(fileName, std::ios::in);
        auto g = generator::generateFromStream(file);
        file.close();
        return g;
    }

    bool equal(const Vertex::VerticesRefsVector& lhs, const StringVec& rhs) {
        if (lhs.size() != rhs.size()) return false;

        for (const gis::Vertex &v : lhs) {
            if (rhs.end() == rhs.find(v.label)) {
                return false;
            }
        }

        return true;
    }

    bool equal(const std::vector<Vertex::VerticesRefsVector>& lhs, std::vector<StringVec>& rhs) {
        if (lhs.size() != rhs.size()) return false;

        for (const auto &comp : lhs) {
            for (size_t j = 0; j < rhs.size(); ++j) {
                if (equal(comp, rhs[j])) {
                    rhs.erase(rhs.begin() + j);
                    break;
                }
            }
        }

        return rhs.empty();
    }
};

BOOST_FIXTURE_TEST_SUITE(AcceptanceTests, AcceptanceTestsFixture)

    BOOST_AUTO_TEST_CASE(DAG_Graph)
    {
        std::vector<StringVec> expected = {
                {"0"}, {"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"10"}, {"11"}, {"12"}
        };
        auto g = readGraph(acceptanceTestsBasePath + dagFile);
        auto scc = g.findStronglyConntectedComponents();
        BOOST_REQUIRE(equal(scc, expected));
    }

    BOOST_AUTO_TEST_CASE(DG_Graph)
    {
        std::vector<StringVec> expected = {
                {"0", "2", "3", "4", "5"}, {"1"}, {"6"}, {"7", "8"}, {"9", "10", "11", "12"}
        };
        auto g = readGraph(acceptanceTestsBasePath + dgFile);
        auto scc = g.findStronglyConntectedComponents();
        BOOST_REQUIRE(equal(scc, expected));
    }

    BOOST_AUTO_TEST_CASE(Full_Graph)
    {
        std::vector<StringVec> expected = {
                {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}
        };
        auto g = readGraph(acceptanceTestsBasePath + fullFile);
        auto scc = g.findStronglyConntectedComponents();
        BOOST_REQUIRE(equal(scc, expected));
    }

    BOOST_AUTO_TEST_CASE(NoEdges_Graph)
    {
        std::vector<StringVec> expected = {
                {"0"}, {"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"10"}, {"11"}, {"12"}
        };
        auto g = readGraph(acceptanceTestsBasePath + noEdgesFile);
        auto scc = g.findStronglyConntectedComponents();
        BOOST_REQUIRE(equal(scc, expected));
    }

    BOOST_AUTO_TEST_CASE(ParallelEdges_Graph)
    {
        std::vector<StringVec> expected = {
                {"0", "2", "3", "4", "5"}, {"1"}, {"6"}, {"7", "8"}, {"9", "10", "11", "12"}
        };
        auto g = readGraph(acceptanceTestsBasePath + parallelEdgesFile);
        auto scc = g.findStronglyConntectedComponents();
        BOOST_REQUIRE(equal(scc, expected));
    }

    BOOST_AUTO_TEST_CASE(SCG_Graph)
    {
        std::vector<StringVec> expected = {
                {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}
        };
        auto g = readGraph(acceptanceTestsBasePath + scgFile);
        auto scc = g.findStronglyConntectedComponents();
        BOOST_REQUIRE(equal(scc, expected));
    }

BOOST_AUTO_TEST_SUITE_END()