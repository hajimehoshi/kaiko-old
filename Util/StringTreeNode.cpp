#include "StringTreeNode.hpp"

#include "Serialization.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

namespace Kaiko {
namespace Util {

struct StringTreeNode::Impl {
  explicit Impl(const std::string& key)
    : key(key) {
  }
  static std::shared_ptr<StringTreeNode> StrToNode(std::string::const_iterator begin,
                                                   std::string::const_iterator end,
                                                   int* readBytesNum) {
    assert(readBytesNum);
    *readBytesNum = 0;
    int keyLength = 0;
    {
      int x = 0;
      keyLength = Serialization::BytesToLength(begin + *readBytesNum, end, &x);
      if (keyLength == 0) {
        // logging
        return std::shared_ptr<StringTreeNode>();
      }
      *readBytesNum += x;
    }
    const std::string key(begin + *readBytesNum, begin + *readBytesNum + keyLength);
    *readBytesNum += keyLength;
    int childNodesNum = 0;
    {
      int x = 0;
      childNodesNum = Serialization::BytesToLength(begin + *readBytesNum, end, &x);
      if (keyLength == 0) {
        // TODO: logging
        return std::shared_ptr<StringTreeNode>();
      }
      *readBytesNum += x;
    }
    std::shared_ptr<StringTreeNode> stringTreeNode(new StringTreeNode(key));
    stringTreeNode->pimpl->childNodes.reserve(childNodesNum);
    for (int i = 0; i < childNodesNum; ++i) {
      int x = 0;
      auto childNode = StrToNode(begin + *readBytesNum, end, &x);
      stringTreeNode->pimpl->childNodes.push_back(childNode);
      *readBytesNum += x;
    }
    return stringTreeNode;
  }
  std::string key;
  Nodes childNodes;
};

std::shared_ptr<StringTreeNode>
StringTreeNode::CreateFromString(const std::string& str) {
  int readBytesNum = 0;
  auto result = StringTreeNode::Impl::StrToNode(str.begin(), str.end(), &readBytesNum);
  if (readBytesNum != static_cast<int>(str.size())) {
    // TODO: logging
    return std::shared_ptr<StringTreeNode>();
  }
  return result;
}

StringTreeNode::StringTreeNode(const std::string& key)
  : pimpl(new Impl(key)) {
}

StringTreeNode::~StringTreeNode() {
}

void
StringTreeNode::AddChildNode(const std::string& key) {
  std::shared_ptr<StringTreeNode> childNode =
    std::shared_ptr<StringTreeNode>(new StringTreeNode(key));
  this->pimpl->childNodes.push_back(childNode);
}

bool
StringTreeNode::Contains(const std::string& key) const {
  return std::find_if(this->pimpl->childNodes.begin(),
                      this->pimpl->childNodes.end(),
                      [&](const std::shared_ptr<StringTreeNode>& t) {
                        return t->GetKey() == key;
                      }) != this->pimpl->childNodes.end();
}

const StringTreeNode::Nodes
StringTreeNode::GetChildNodes() const {
  return this->pimpl->childNodes;
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
  std::for_each(this->pimpl->childNodes.begin(),
                this->pimpl->childNodes.end(),
                [&](const std::shared_ptr<StringTreeNode>& t) {
                  result.append((t)->ToString());
                });
  return result;
}

}
}
