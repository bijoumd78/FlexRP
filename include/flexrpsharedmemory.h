#ifndef FLEXRPSHAREDMEMORY_H
#define FLEXRPSHAREDMEMORY_H

#include <vector>
#include <string>

namespace FlEXRP {

struct FlexRPSharedMemory
{
    static void createSharedMemory(const std::vector<std::string> &v);
    static void getReconmoduleProperty(std::vector<std::string> &v, const size_t n);
    static bool to_bool(std::string &str);
};

}
#endif // FLEXRPSHAREDMEMORY_H
