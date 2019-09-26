#include "flexrpsharedmemory.h"
#include <boost/lockfree/spsc_queue.hpp> // ring buffer
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <algorithm>

namespace FlEXRP {

namespace bip = boost::interprocess;
using  char_alloc    = bip::allocator<char, bip::managed_shared_memory::segment_manager>;
using  shared_string = bip::basic_string<char, std::char_traits<char>, char_alloc >;
using  ring_buffer   = boost::lockfree::spsc_queue<shared_string, boost::lockfree::capacity<200> >;

void FlexRPSharedMemory::createSharedMemory(const std::vector<std::string> &v)
{
    bip::managed_shared_memory segment(bip::open_or_create, "FlexRPSharedMemory", 65536);
    char_alloc char_alloc(segment.get_segment_manager());

    // Ringbuffer fully constructed in shared memory. The element strings are
    // also allocated from the same shared memory segment. This vector can be
    // safely accessed from other processes.
    ring_buffer *queue = segment.find_or_construct<ring_buffer>("queue")();

    for(const auto &e : v)
        queue->push(shared_string(e.c_str(), char_alloc));
}

void FlexRPSharedMemory::getReconmoduleProperty(std::vector<std::string> &v, const size_t n)
{
    bip::managed_shared_memory segment(bip::open_or_create, "FlexRPSharedMemory", 65536);
    char_alloc char_alloc(segment.get_segment_manager());

    ring_buffer *queue = segment.find_or_construct<ring_buffer>("queue")();

    for(size_t i = 0 ; i < n; ++i)
    {
        shared_string element(char_alloc);
        if (queue->pop(element)){
            v.emplace_back(element.data());
        }
    }
}

bool FlexRPSharedMemory::to_bool(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

}
