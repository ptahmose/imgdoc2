# SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
#
# SPDX-License-Identifier: MIT

set(ImgDoc2SqlLiteExtension_Srcfiles 
    "imgdoc2sqliteextension.h"
    "imgdoc2sqliteextension.cpp")

add_library(imgdoc2sqliteextension SHARED ${ImgDoc2SqlLiteExtension_Srcfiles} $<TARGET_OBJECTS:LibSqlLiteExtension>)

set_target_properties(imgdoc2sqliteextension PROPERTIES INCLUDE_DIRECTORIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/sqlite3>")

