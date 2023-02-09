#ifndef FACTORY_FACTORY_H
#define FACTORY_FACTORY_H
#pragma once

#include <string>
#include <map>
#include <vector>

class Object {
 public:
  virtual std::string ToString() const = 0;

  virtual ~Object() {}
};

class ObjectChild : public Object {
 private:
  std::string id;
 public:
  explicit ObjectChild(std::string id_name) : id(id_name) {}

  virtual std::string ToString() const {
    return id;
  }
};

class Factory {
 private:
  std::map<std::string, Object *(*)()> containers_new;
  std::vector<std::string> containers_standart;

 public:
  Factory() {
    containers_standart.push_back("apple!");
    containers_standart.push_back("list");
    containers_standart.push_back("yet another identifier");
  }

  Object *Create(const std::string &class_id) {
    if (containers_new.find(class_id) != containers_new.end()) {
      return containers_new[class_id]();
    }
    for (auto i : containers_standart) {
      if (i == class_id)
        return new ObjectChild(class_id);
    }
    return nullptr;
  }

  void Register(const std::string &class_id, Object *(*instance_creator)()) {
    containers_new[class_id] = instance_creator;
  }
};
#endif
