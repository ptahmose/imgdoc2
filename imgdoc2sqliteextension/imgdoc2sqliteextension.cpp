#include "../libSqliteExtension/sqlite_custom_functions.h"

#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT1

#ifdef _WIN32
__declspec(dllexport)
#endif
int sqlite3_rot_init(
  sqlite3* database,
  char** pzErrMsg,
  const sqlite3_api_routines* pApi)
{
    int return_code = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void)pzErrMsg;  /* Unused parameter */

    return_code = sqlite3_rtree_query_callback(
     database,
     GetCustomFunctionName(SqliteExtensionCustomFunction::RTree_PlaneAabb3D),
     Plane3d_Query,
     nullptr,
     nullptr);

    /*  rc = sqlite3_create_function(db, "rot13", 1,
                       SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
                       0, rot13func, 0, 0);
      if (rc == SQLITE_OK) {
          rc = sqlite3_create_collation(db, "rot13", SQLITE_UTF8, 0, rot13CollFunc);
      }*/
    return return_code;
}
