#include <map>
#include <vector>
#include <string>
#include <functional>

using std::string;
using std::map;
using std::vector;

class GameDatabase {
 private:
  std::map<ObjectId, GameObject, std::greater<ObjectId>> dataBase_;
  using innerType = map<ObjectId, GameObject>;

 public:
  GameDatabase() = default;

  void Insert(ObjectId id, std::string name, size_t x, size_t y) {
    GameObject obj;
    obj.name = name;
    obj.id = id;
    obj.x = x;
    obj.y = y;
    dataBase_[id] = obj;
  }

  void Remove(ObjectId id) {
    auto it = dataBase_.find(id);
    if (it != std::end(dataBase_))
      dataBase_.erase(it);
  }

  vector<GameObject> DataByName(string name) const {
    vector<GameObject> ret;

    for (innerType::const_iterator i = dataBase_.begin();
         i != dataBase_.end(); ++i)
      if (i->second.name == name)
        ret.push_back(i->second);

    return ret;
  }

  vector<GameObject> DataByPosition(size_t x, size_t y) const {
    vector<GameObject> ret;

    for (auto const &elem : dataBase_)
      if (elem.second.x == x && elem.second.y == y)
        ret.push_back(elem.second);

    return ret;
  }

  vector<GameObject> Data() const {
    vector<GameObject> ret;

    for (auto const &elem : dataBase_)
      ret.push_back(elem.second);

    return ret;
  }
};
