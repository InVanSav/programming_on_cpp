#include <map>

using std::map;

template<typename Vertex>
class BfsVisitor {
 private:
  static map<Vertex, int> distances;
  static map<Vertex, Vertex> parents;
  static bool first_vertex;
  static Vertex current_vertex;

 public:
  BfsVisitor() {
    distances = map<Vertex, int>();
    parents = map<Vertex, Vertex>();
    first_vertex = true;
  }

  void ExamineVertex(const Vertex& vertex) {
    if (first_vertex) {
      first_vertex = false;
      distances[vertex] = 0;
      parents[vertex] = vertex;
      current_vertex = vertex;
      return;
    }
    current_vertex = vertex;
  }

  void DiscoverVertex(const Vertex& vertex) {
    distances[vertex] = distances[current_vertex]+1;
    parents[vertex] = current_vertex;
  }

  size_t DistanceTo(const Vertex& target) const {
    return distances.at(target);
  }
  Vertex Parent(const Vertex& vertex) const {
    return parents.at(vertex);
  }
};
template <typename T>
map<T, int> BfsVisitor<T>::distances = map<T, int>();

template <typename T>
map<T, T> BfsVisitor<T>::parents = map<T, T>();

template <typename T>
bool BfsVisitor<T>::first_vertex = true;

template <typename T>
T BfsVisitor<T>::current_vertex = T();
