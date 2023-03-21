# SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
#
# SPDX-License-Identifier: MIT

set(LibSqlLiteExtension_Srcfiles 
    "sqlite_custom_functions.h"
    "sqlite_custom_functions.cpp")

add_library(LibSqlLiteExtension OBJECT ${LibSqlLiteExtension_Srcfiles})

# shared libraries need PIC
set_property(TARGET LibSqlLiteExtension PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(LibSqlLiteExtension PROPERTIES INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/sqlite3>")
