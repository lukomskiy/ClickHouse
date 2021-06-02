#include <Common/IOMetrics.h>
#include <iostream>
#include <gtest/gtest.h>

TEST(IOMetrics, SimpleTest) 
{
    std::cout << "IOMetrics\n";
    for (int i = 0; i < 10; i++) 
    {
        DB::IOMetrics a;
        DB::IOMetrics::Data x = a.get();
        for (auto &it : x.dev_stats) 
        {
            std::cout << it.first << "\n";
            auto st = it.second;
            std::cout << "Read complete : " << st.read_complete << "\n";
            std::cout << "Read merge : " << st.read_merge << "\n";
            std::cout << "Read sectors : " << st.read_sectors << "\n";
            std::cout << "Read time : " << st.read_time << "\n";
            std::cout << "Read per sec : " << st.read_per_sec << "\n";
            std::cout << "Read queue size : " << st.read_queue_size << "\n";
            std::cout << "Write complete : " << st.write_complete << "\n";
            std::cout << "Write merge : " << st.write_merge << "\n";
            std::cout << "Write sectors : " << st.write_sectors << "\n";
            std::cout << "Write time : " << st.write_time << "\n";
            std::cout << "Write per sec : " << st.write_per_sec << "\n";
            std::cout << "Write queue size : " << st.write_queue_size << "\n";
            std::cout << "Discard complete : " << st.discard_complete << "\n";
            std::cout << "Discard merge : " << st.discard_merge << "\n";
            std::cout << "Discard sectors : " << st.discard_sectors << "\n";
            std::cout << "Discard time : " << st.discard_time << "\n";
            std::cout << "Discard per sec : " << st.discard_per_sec << "\n";
            std::cout << "Discard queue size : " << st.discard_queue_size << "\n";
        }
        std::cout << "\n";
    }
} 
