#pragma once

#include <string>
#include <vector>

namespace gis {

class Vertex {
public:
    using VerticesRefsVector = std::vector<std::reference_wrapper<Vertex>>;

    Vertex() = default;
    Vertex(const Vertex &other) : Vertex(other, NoNeighbours) { neighbours = other.neighbours; }
    Vertex & operator =(const Vertex &) = default;
    Vertex(Vertex &&) = default;
    Vertex & operator =(Vertex &&) = default;
    ~Vertex() = default;

    Vertex copyWithoutNeighbours() const {
        return Vertex(*this, NoNeighbours);
    }

    std::string label;
    VerticesRefsVector neighbours;

private:
    enum NoNeighoursTag { NoNeighbours };
    Vertex(const Vertex &other, NoNeighoursTag) : label(other.label) {}
};

} // namespace gis
