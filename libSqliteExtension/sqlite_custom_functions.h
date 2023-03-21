#pragma once

#include <cstdint>

enum class SqliteExtensionCustomFunction : std::uint8_t
{
    RTree_PlaneAabb3D = 1,
};

const char* GetCustomFunctionName(SqliteExtensionCustomFunction custom_function);

struct sqlite3_rtree_query_info;

int Plane3d_Query(sqlite3_rtree_query_info* info);
