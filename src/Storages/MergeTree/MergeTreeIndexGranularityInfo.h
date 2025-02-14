#pragma once

#include <optional>
#include <base/types.h>
#include <Storages/MergeTree/MergeTreeDataPartType.h>
#include <Disks/IDisk.h>
#include <Storages/MergeTree/IDataPartStorage.h>

namespace DB
{

class MergeTreeData;

/// Meta information about index granularity
struct MergeTreeIndexGranularityInfo
{
public:
    /// Marks file extension '.mrk' or '.mrk2'
    String marks_file_extension;

    /// Is stride in rows between marks non fixed?
    bool is_adaptive = false;

    /// Fixed size in rows of one granule if index_granularity_bytes is zero
    size_t fixed_index_granularity = 0;

    /// Approximate bytes size of one granule
    size_t index_granularity_bytes = 0;

    MergeTreeIndexGranularityInfo(const MergeTreeData & storage, MergeTreeDataPartType type_);

    MergeTreeIndexGranularityInfo(MergeTreeDataPartType type_, bool is_adaptive_, size_t index_granularity_, size_t index_granularity_bytes_);

    void changeGranularityIfRequired(const DataPartStoragePtr & data_part_storage);

    String getMarksFilePath(const String & path_prefix) const
    {
        return path_prefix + marks_file_extension;
    }

    size_t getMarkSizeInBytes(size_t columns_num = 1) const;

    static std::optional<std::string> getMarksExtensionFromFilesystem(const DataPartStoragePtr & data_part_storage);

private:
    MergeTreeDataPartType type;
    void setAdaptive(size_t index_granularity_bytes_);
    void setNonAdaptive();
};

constexpr inline auto getNonAdaptiveMrkExtension() { return ".mrk"; }
constexpr inline auto getNonAdaptiveMrkSizeWide() { return sizeof(UInt64) * 2; }
constexpr inline auto getAdaptiveMrkSizeWide() { return sizeof(UInt64) * 3; }
inline size_t getAdaptiveMrkSizeCompact(size_t columns_num);
std::string getAdaptiveMrkExtension(MergeTreeDataPartType part_type);

}
