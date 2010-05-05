#ifndef KAIKO_UTIL_STRINGTREENODE_HPP
#define KAIKO_UTIL_STRINGTREENODE_HPP

#include "IEnumerable.hpp"

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace Kaiko {
namespace Util {

class StringTreeNode : private boost::noncopyable{
public:
  static boost::shared_ptr<StringTreeNode> CreateFromString(const std::string& str);
  explicit StringTreeNode(const std::string& key);
  void AddChildNode(const std::string& key);
  const IEnumerable<boost::shared_ptr<StringTreeNode>>& GetChildNodes() const;
  const std::string& GetKey() const;
  std::string ToString() const;
private:
  struct Impl;
  boost::scoped_ptr<Impl> pimpl;
};

}
}

#endif
