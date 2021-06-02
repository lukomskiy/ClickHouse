#include <vector>
#include <utility>

#include <Core/Types.h>

namespace DB
{

class IOMetrics
{
public:
    
    struct Data
    {
        struct iostats
        {
            uint64_t read_complete;
            uint64_t read_merge;
            uint64_t read_sectors;
            uint64_t read_time;
            float read_per_sec;
            float read_queue_size;
            uint64_t write_complete;
            uint64_t write_merge;
            uint64_t write_sectors;
            uint64_t write_time;
            float write_per_sec;
            float write_queue_size;
            uint64_t discard_complete;
            uint64_t discard_merge;
            uint64_t discard_sectors;
            uint64_t discard_time;
            float discard_per_sec;
            float discard_queue_size;
        };

        std::vector<std::pair<String, iostats>> dev_stats;
    };

    IOMetrics();
    ~IOMetrics();

    Data get() const;

private:
};

}
