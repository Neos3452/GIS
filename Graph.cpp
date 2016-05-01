#include "Graph.h"

#include <cassert>
#include <deque>
#include <functional>
#include <memory>

namespace gis {

using VertexStack = std::deque<std::reference_wrapper<Vertex>>;

size_t Graph::vertexIndex(const Vertex &v) const
{
    assert(std::addressof(v) >= vertices.data());
    const size_t index = std::addressof(v) - vertices.data();
    assert(index < vertices.size());
    return index;
}

void Graph::rewireReferences(const Graph &)
{
    // TODO
    assert(false);
}

Graph Graph::transposedGraph() const
{
    Graph result;
    result.vertices.reserve(vertices.size());

    // so the indices are the same
    auto vit = vertices.cbegin();
    std::generate_n(std::back_inserter(result.vertices), vertices.size(), [&vit](){
        return (vit++)->copyWithoutNeighbours();
    });
    assert(vertices.size() == result.vertices.size());

    for (const auto &v : vertices) {
        auto &resultVertex = result.vertices[vertexIndex(v)];

        // weak check
        assert(v.label == resultVertex.label);

        for (const Vertex &n : v.neighbours) {
            result.vertices[vertexIndex(n)].neighbours.emplace_back(resultVertex);
        }
    }

    return result;
}

std::vector<Vertex::VerticesRefsVector> Graph::findStronglyConntectedComponents()
{
    auto markers = std::make_unique<bool[]>(vertices.size());
    std::fill(markers.get(), markers.get() + vertices.size(), false);

    const auto marker = [&markers, this](const Vertex &v) -> auto& {
        return *(markers.get() + vertexIndex(v));
    };

    VertexStack stack;

    const std::function<void(Vertex &)> createStack = [&](auto &v)
    {
        if (!marker(v)) {
            std::for_each(v.neighbours.begin(), v.neighbours.end(), createStack);
            stack.emplace_back(v);
            marker(v) = true;
        }
    };

    // random means first or truly random?
    std::for_each(vertices.begin(), vertices.end(), createStack);

    auto transposed = transposedGraph();

    std::fill(markers.get(), markers.get() + vertices.size(), false);

    std::vector<Vertex::VerticesRefsVector> result;

    std::for_each(stack.rbegin(), stack.rend(), [&](auto &v) {
        if (!marker(v)) {
            result.resize(result.size() + 1);
            auto &component = result.back();

            const std::function<void(Vertex &)> findComponents = [&](auto &v)
            {
                if (!marker(v)) {
                    component.emplace_back(v);
                    // here is the tricky part, we want to iterate in order previously stored in stack
                    // but over neighbours from tansposed graph. So we need to find vertex's neighbours of
                    // transposed graph using index from current graph and then use the neighbour to get
                    // index from transposed graph so we can use vertex from original graph
                    for (const auto &transposedNeighbour : transposed.vertices[vertexIndex(v)].neighbours) {
                        findComponents(vertices[transposed.vertexIndex(transposedNeighbour)]);
                    }
                    marker(v) = true;
                }
            };
            findComponents(v);
        }
    });

    return result;
}

} // namespace gis
