#pragma once

#include "Vertex.h"

#include <vector>

namespace gis {

/*
 * Graph holds (physically) all vertices that belong to it. This allows for
 * very interesting manipulations using references only.
 */
class Graph {
public:
    using VertexVector = std::vector<Vertex>;

    Graph() = default;

    Graph(const Graph &other) : Graph(other.vertices) { rewireReferences(other); }
    Graph & operator =(const Graph &other) { vertices = other.vertices; rewireReferences(other); return *this; }

    /*
     * Below makes defaulting move operations OK (ie. vertices' neighbours
     * will properly point to the new object)
     *
     * After container move assignment (overload (2)), unless elementwise move
     * assignment is forced by incompatible allocators, references, pointers,
     * and iterators (other than the end iterator) to other remain valid, but
     * refer to elements that are now in *this. The current standard makes this
     * guarantee via the blanket statement in
     * §23.2.1[container.requirements.general]/12, and a more direct guarantee
     * is under consideration via LWG 2321.
     * http://en.cppreference.com/w/cpp/container/vector/operator%3D
     */
    Graph(Graph &&) = default;
    Graph & operator =(Graph &&) = default;

    ~Graph() = default;

    // Returns a new graph which is a transposition of this
    Graph transposedGraph() const;

    // Returns list of strongly connected components.
    // Each component is a list of references to a vertex belonging to this component.
    std::vector<Vertex::VerticesRefsVector> findStronglyConntectedComponents();

private:
    Graph(const VertexVector &vertices) : vertices(vertices) {}

    // Finds an index of a vertex which belongs to this graph
    size_t vertexIndex(const Vertex &) const;

    // Rewires neighbours after vertices has been copied from other graph
    void rewireReferences(const Graph &from);

public:
    VertexVector vertices;
};

} // namespace gis
