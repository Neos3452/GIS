#pragma once

#include <string>
#include <functional>
#include <vector>

namespace gis {

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

    inline Vertex copyWithoutNeighbours() const {
        return Vertex(*this, NoNeighbours);
    }

    std::string label;
    VerticesRefsVector neighbours;

private:
    enum NoNeighoursTag { NoNeighbours };
    Vertex(const Vertex &other, NoNeighoursTag) : label(other.label) {}
};

} // namespace gis
