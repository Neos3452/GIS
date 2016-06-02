#pragma once

#include <string>
#include <functional>
#include <vector>

namespace gis {

/*
 * Represents a single vertex in a graph. It holds all it's neighbours using
 * std::reference_wrapper. This assures that neighbours cannot be null while
 * allowing reassigment.
 */
class Vertex {
public:
    using VerticesRefsVector = std::vector<std::reference_wrapper<Vertex>>;

    Vertex() = default;
    explicit Vertex(std::string label) : label(std::move(label)) {}
    Vertex(const Vertex &other) : Vertex(other, NoNeighbours) { neighbours = other.neighbours; }
    Vertex & operator =(const Vertex &) = default;
    Vertex(Vertex &&) = default;
    Vertex & operator =(Vertex &&) = default;
    ~Vertex() = default;

    // helper for transposition algorithm
    inline Vertex copyWithoutNeighbours() const {
        return Vertex(*this, NoNeighbours);
    }

    std::string label;
    VerticesRefsVector neighbours;

private:
    // This is a trick so we can delegate a ctr
    enum NoNeighoursTag { NoNeighbours };
    Vertex(const Vertex &other, NoNeighoursTag) : label(other.label) {}
};

} // namespace gis
