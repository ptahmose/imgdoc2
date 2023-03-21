#include "sqlite_custom_functions.h"

#include <sqlite3.h>
#include <stdexcept>
#include <string>

const char* GetCustomFunctionName(SqliteExtensionCustomFunction custom_function)
{
    switch (custom_function)
    {
    case SqliteExtensionCustomFunction::RTree_PlaneAabb3D:
        return "Plane3d_Query";
    default:
        throw std::invalid_argument("Unknown custom function.");
    }
}


int Plane3d_Query(sqlite3_rtree_query_info* info)
{
    return SQLITE_ERROR;
}
