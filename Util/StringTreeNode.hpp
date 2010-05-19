#ifndef KAIKO_UTIL_STRINGTREENODE_HPP
#define KAIKO_UTIL_STRINGTREENODE_HPP

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Kaiko {
namespace Util {

class StringTreeNode : private boost::noncopyable {
public:
  typedef std::vector<std::shared_ptr<StringTreeNode>> Nodes;
  static std::shared_ptr<StringTreeNode> CreateFromString(const std::string& str);
  explicit StringTreeNode(const std::string& key);
  ~StringTreeNode();
  void AddChildNode(const std::string& key);
  const Nodes& GetChildNodes() const;
  const std::string& GetKey() const;
  std::string ToString() const;
private:
  struct Impl;
  const std::unique_ptr<Impl> pimpl;
};

}
}

#endif
