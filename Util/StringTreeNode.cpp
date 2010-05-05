#include "StringTreeNode.hpp"

#include "ContainerEnumerable.hpp"
#include "Serialization.hpp"

#include <vector>

namespace Kaiko {
namespace Util {

struct StringTreeNode::Impl {
  typedef std::vector< boost::shared_ptr<StringTreeNode> > Nodes;
  explicit Impl(const std::string& key)
    : key(key) {
    this->childNodesEnumerable.reset(new ContainerEnumerable<Nodes>(this->childNodes));
  }
  std::string key;
  Nodes childNodes;
  boost::scoped_ptr< ContainerEnumerable<Nodes> > childNodesEnumerable;
};

boost::shared_ptr<StringTreeNode>
StringTreeNode::CreateFromString(const std::string& str) {
  return boost::shared_ptr<StringTreeNode>();
}

StringTreeNode::StringTreeNode(const std::string& key)
  : pimpl(new Impl(key)) {
}

void
StringTreeNode::AddChildNode(const std::string& key) {
  boost::shared_ptr<StringTreeNode> childNode =
    boost::shared_ptr<StringTreeNode>(new StringTreeNode(key));
  this->pimpl->childNodes.push_back(childNode);
}

const IEnumerable< boost::shared_ptr<StringTreeNode> >&
StringTreeNode::GetChildNodes() const {
  return *this->pimpl->childNodesEnumerable;
}

const std::string&
StringTreeNode::GetKey() const {
  return this->pimpl->key;
}

std::string
StringTreeNode::ToString() const {
  std::string result;
  result.append(Serialization::LengthToBytes(this->GetKey().size()));
  result.append(this->GetKey());
  result.append(Serialization::LengthToBytes(this->pimpl->childNodes.size()));
  for (auto it = this->pimpl->childNodes.begin();
       it != this->pimpl->childNodes.end();
       ++it) {
    result.append((*it)->ToString());
  }
  return result;
}

}
}
