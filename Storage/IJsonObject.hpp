#ifndef KAIKO_STORAGE_IJSONOBJECT_HPP
#define KAIKO_STORAGE_IJSONOBJECT_HPP

#include <string>

namespace Kaiko {
namespace Storage {

enum JsonObjectType {
  JsonObjectObject,
  JsonObjectTrue,
  JsonObjectFalse,
  JsonObjectNull,
  JsonObjectInteger,
  JsonObjectFloat,
  JsonObjectString,
};

class IJsonObject {
public:
  std::string ToString() = 0;
};

}
}

#endif
