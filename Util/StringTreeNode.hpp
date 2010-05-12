#ifndef KAIKO_UTIL_STRINGTREENODE_HPP
#define KAIKO_UTIL_STRINGTREENODE_HPP

#include "IEnumerable.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

namespace Kaiko {
namespace Util {

class StringTreeNode : private boost::noncopyable {
public:
  static std::shared_ptr<StringTreeNode> CreateFromString(const std::string& str);
  explicit StringTreeNode(const std::string& key);
  ~StringTreeNode();
  void AddChildNode(const std::string& key);
  bool Contains(const std::string& key) const;
  const IEnumerable<std::shared_ptr<StringTreeNode>>& GetChildNodes() const;
  const std::string& GetKey() const;
  std::string ToString() const;
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl;
};

}
}

#endif
