#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <filesystem>

#include "IOMetrics.h"

#include <IO/ReadBufferFromMemory.h>
#include <IO/ReadBufferFromFile.h>
#include <IO/ReadHelpers.h>

namespace DB
{

namespace
{
    template<typename T>
    void readIntAndSkipWhitespaceIfAny(T& x, ReadBuffer& buf) 
    {
        readIntText(x, buf);
        skipWhitespaceIfAny(buf);
    }
}   

String iostat_path = "/sys/block";

static constexpr size_t READ_BUF_SIZE = 1 << 16;

IOMetrics::IOMetrics() {}

IOMetrics::~IOMetrics() {}

IOMetrics::Data IOMetrics::get() const
{
    IOMetrics::Data data;

    for (auto& it : std::filesystem::directory_iterator(iostat_path)) 
    {
        String filename = it.path().string() + "/stat";
        String dev_name = it.path().string().substr(iostat_path.size(), it.path().string().size() - iostat_path.size());
        ReadBufferFromFile buf(filename, READ_BUF_SIZE, O_RDONLY | O_CLOEXEC);

        IOMetrics::Data::iostats cur;
        
        readIntAndSkipWhitespaceIfAny(cur.read_complete, buf);
        readIntAndSkipWhitespaceIfAny(cur.read_merge, buf);
        readIntAndSkipWhitespaceIfAny(cur.read_sectors, buf);
        readIntAndSkipWhitespaceIfAny(cur.read_time, buf);
        readIntAndSkipWhitespaceIfAny(cur.write_complete, buf);
        readIntAndSkipWhitespaceIfAny(cur.write_merge, buf);
        readIntAndSkipWhitespaceIfAny(cur.write_sectors, buf);
        readIntAndSkipWhitespaceIfAny(cur.write_time, buf);
        if (cur.read_time > 0)
        {
            cur.read_per_sec = static_cast<float>(cur.read_complete) / cur.read_time * 1000;
            cur.read_queue_size = static_cast<float>(cur.read_merge - cur.read_complete) / cur.read_time * 1000;
        }
        if (cur.write_time > 0)
        {
            cur.write_per_sec = static_cast<float>(cur.write_complete) / cur.write_time * 1000;
            cur.write_queue_size = static_cast<float>(cur.write_merge - cur.write_complete) / cur.write_time * 1000;
        } 

        uint32_t unused;
        readIntAndSkipWhitespaceIfAny(unused, buf);
        readIntAndSkipWhitespaceIfAny(unused, buf);
        readIntAndSkipWhitespaceIfAny(unused, buf);

        readIntAndSkipWhitespaceIfAny(cur.discard_complete, buf);
        readIntAndSkipWhitespaceIfAny(cur.discard_merge, buf);
        readIntAndSkipWhitespaceIfAny(cur.discard_sectors, buf);
        readIntAndSkipWhitespaceIfAny(cur.discard_time, buf);
        if (cur.discard_time > 0)
        {
            cur.discard_per_sec = static_cast<float>(cur.discard_complete) / cur.discard_time * 1000;
            cur.discard_queue_size = static_cast<float>(cur.discard_merge - cur.discard_complete) / cur.discard_time * 1000;
        }

        data.dev_stats.push_back(std::make_pair(dev_name, cur));
    }
    
    return data;
}

}
