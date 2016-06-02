#ifdef GIS_MEASURE_STACK
#include "MemoryHooks.h"
#endif

// Update max stack size whenever it's reasonable. I don't think it can be
// automated.
#ifdef GIS_MEASURE_STACK
#define GIS_UPDATE_MAX_STACK_SIZE memory_measure::updateMaximumStackSize()
#else
#define GIS_UPDATE_MAX_STACK_SIZE
#endif

#include "Graph.h"

#include <cassert>
#include <deque>
#include <algorithm>
#include <functional>
#include <memory>

namespace gis {

using VertexStack = std::deque<std::reference_wrapper<Vertex>>;

size_t Graph::vertexIndex(const Vertex &v) const
{
    // Just get the address of a vertex and subtract it's array's base (vector::data)
    assert(std::addressof(v) >= vertices.data());
    const size_t index = static_cast<size_t>(std::addressof(v) - vertices.data());
    assert(index < vertices.size());
    GIS_UPDATE_MAX_STACK_SIZE;
    return index;
}

void Graph::rewireReferences(const Graph &old)
{
    // Get the reference to point to vertex of the same index but in the new graph.
    for (auto &v : vertices) {
        for (auto &n : v.neighbours) {
            n = vertices[old.vertexIndex(n)];
            GIS_UPDATE_MAX_STACK_SIZE;
        }
    }
}

Graph Graph::transposedGraph() const
{
    Graph result;
    result.vertices.reserve(vertices.size());

    // Indices will be the same, so we can copy all the vertices to the new graph
    auto vit = vertices.cbegin();
    std::generate_n(std::back_inserter(result.vertices), vertices.size(), [&vit](){
        GIS_UPDATE_MAX_STACK_SIZE;
        return (vit++)->copyWithoutNeighbours();
    });
    assert(vertices.size() == result.vertices.size());

    for (const auto &v : vertices) {
        auto &resultVertex = result.vertices[vertexIndex(v)];

        // weak check
        assert(v.label == resultVertex.label);

        // for each vertex we add reversed edge to the new graph
        for (const Vertex &n : v.neighbours) {
            result.vertices[vertexIndex(n)].neighbours.emplace_back(resultVertex);
            GIS_UPDATE_MAX_STACK_SIZE;
        }
        GIS_UPDATE_MAX_STACK_SIZE;
    }

    GIS_UPDATE_MAX_STACK_SIZE;
    return result;
}

std::vector<Vertex::VerticesRefsVector> Graph::findStronglyConntectedComponents()
{
    auto markers = std::make_unique<bool[]>(vertices.size());
    std::fill(markers.get(), markers.get() + vertices.size(), false);

    // helper function, so we can call marker()
    // returns reference to a marker for a given vertex
    const auto marker = [&markers, this](const Vertex &v) -> auto& {
        GIS_UPDATE_MAX_STACK_SIZE;
        return *(markers.get() + vertexIndex(v));
    };

    VertexStack stack;

    // createStack fills the stack using dfs as in SCC algorithm. A vertex is
    // added when all it's neighbours has been visited.
    // markers keep track of visited vertices.
    const std::function<void(Vertex &)> createStack = [&](auto &v)
    {
        if (!marker(v)) {
            marker(v) = true;
            std::for_each(v.neighbours.begin(), v.neighbours.end(), createStack);
            stack.emplace_back(v);
        }
        GIS_UPDATE_MAX_STACK_SIZE;
    };

    // It is assumed that random means "it doesn't matter" so we pick the first one
    std::for_each(vertices.begin(), vertices.end(), createStack);

    auto transposed = transposedGraph();

    std::fill(markers.get(), markers.get() + vertices.size(), false);

    std::vector<Vertex::VerticesRefsVector> result;

    std::for_each(stack.rbegin(), stack.rend(), [&](auto &v) {
        if (!marker(v)) {
            // We haven't visited this vertex yet, means it's a new component.
            result.resize(result.size() + 1);
            auto &component = result.back();

            const std::function<void(Vertex &)> findComponents = [&](auto &v)
            {
                // If a vertex has already been visited, it means it already belongs to some component
                // and we should consider it.
                if (!marker(v)) {
                    marker(v) = true;
                    component.emplace_back(v);
                    // Here is the tricky part, we want to iterate in order previously stored in stack
                    // but over neighbours from transposed graph. So we need to find vertex's neighbours of
                    // transposed graph using index from current graph and then use the neighbour to get
                    // index from transposed graph so we can use vertex from original graph.
                    for (const auto &transposedNeighbour : transposed.vertices[vertexIndex(v)].neighbours) {
                        findComponents(vertices[transposed.vertexIndex(transposedNeighbour)]);
                    }
                }
                GIS_UPDATE_MAX_STACK_SIZE;
            };
            findComponents(v);
            GIS_UPDATE_MAX_STACK_SIZE;
        }
    });

    GIS_UPDATE_MAX_STACK_SIZE;
    return result;
}

} // namespace gis
