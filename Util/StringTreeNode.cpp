#include "StringTreeNode.hpp"

#include "Serialization.hpp"

#include <algorithm>
#include <cassert>
#include <tuple>
#include <vector>

namespace Kaiko {
namespace Util {

struct StringTreeNode::Impl {
  explicit Impl(const std::string& key)
    : key(key) {
  }
  static std::tuple<std::shared_ptr<StringTreeNode>, int>
  StrToNode(const std::string::const_iterator& begin, const std::string::const_iterator& end) {
    int readBytesNum = 0;
    int keyLength = 0;
    {
      int readBytesNum2;
      try {
        std::tie(keyLength, readBytesNum2) = Serialization::BytesToLength(begin + readBytesNum, end);
      } catch (const Util::Exception&) {
        // TODO: logging
        return std::make_tuple(std::shared_ptr<StringTreeNode>(), 0);
      }
      if (keyLength == 0) {
        // TODO: logging
        return std::make_tuple(std::shared_ptr<StringTreeNode>(), 0);
      }
      readBytesNum += readBytesNum2;
    }
    const std::string key(begin + readBytesNum, begin + readBytesNum + keyLength);
    readBytesNum += keyLength;
    int childNodesNum = 0;
    {
      int readBytesNum2;
      try {
        std::tie(childNodesNum, readBytesNum2) = Serialization::BytesToLength(begin + readBytesNum, end);
      } catch (const Util::Exception&) {
        // TODO: logging
        return std::make_tuple(std::shared_ptr<StringTreeNode>(), 0);
      }
      if (keyLength == 0) {
        // TODO: logging
        return std::make_tuple(std::shared_ptr<StringTreeNode>(), 0);
      }
      readBytesNum += readBytesNum2;
    }
    std::shared_ptr<StringTreeNode> stringTreeNode(new StringTreeNode(key));
    stringTreeNode->pimpl->childNodes.reserve(childNodesNum);
    for (int i = 0; i < childNodesNum; ++i) {
      std::shared_ptr<StringTreeNode> childNode;
      int readBytesNum2;
      std::tie(childNode, readBytesNum2) = StrToNode(begin + readBytesNum, end);
      stringTreeNode->pimpl->childNodes.push_back(childNode);
      readBytesNum += readBytesNum2;
    }
    return std::make_tuple(stringTreeNode, readBytesNum);
  }
  std::string key;
  Nodes childNodes;
};

std::shared_ptr<StringTreeNode>
StringTreeNode::CreateFromString(const std::string& str) {
  std::shared_ptr<StringTreeNode> result;
  int readBytesNum = 0;
  std::tie(result, readBytesNum) = StringTreeNode::Impl::StrToNode(str.begin(), str.end());
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

const StringTreeNode::Nodes&
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
