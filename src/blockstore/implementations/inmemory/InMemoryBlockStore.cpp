#include <blockstore/implementations/inmemory/InMemoryBlock.h>
#include <blockstore/implementations/inmemory/InMemoryBlockStore.h>

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::mutex;
using std::lock_guard;

namespace blockstore {
namespace inmemory {

InMemoryBlockStore::InMemoryBlockStore()
 : _blocks() {}

unique_ptr<Block> InMemoryBlockStore::create(const Key &key, size_t size) {
  auto insert_result = _blocks.emplace(key.AsString(), size);

  if (!insert_result.second) {
    return nullptr;
  }

  //Return a copy of the stored InMemoryBlock
  return make_unique<InMemoryBlock>(insert_result.first->second);
}

unique_ptr<Block> InMemoryBlockStore::load(const Key &key) {
  //Return a copy of the stored InMemoryBlock
  try {
    return make_unique<InMemoryBlock>(_blocks.at(key.AsString()));
  } catch (const std::out_of_range &e) {
    return nullptr;
  }
}

}
}
