#ifndef FLEXRPSHAREDMEMORY_H
#define FLEXRPSHAREDMEMORY_H

#include <string>
#include <vector>

namespace FlexRP {

struct FlexRPSharedMemory {
  static void createSharedMemory(const std::vector<std::string> &v);
  static void getReconmoduleProperty(std::vector<std::string> &v,
                                     const size_t n);
  static bool to_bool(std::string &str);
};

}  // namespace FlexRP
#endif  // FLEXRPSHAREDMEMORY_H
