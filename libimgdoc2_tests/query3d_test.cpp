// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <array>
#include "../libimgdoc2/inc/imgdoc2.h"

using namespace std;
using namespace imgdoc2;
using namespace testing;

/// Creates a new in-memory document with the following characteristics: We have 10x10x10 bricks,
/// each width=height=depth=10, in a checkerboard-arrangement of 10 row, 10 columns and 10 "columns in z-direction". 
/// Each brick has an M-index, starting to count from 1.
/// \param  use_spatial_index   True if the document is to use a spatial index.
/// \returns                    The newly created in-memory "checkerboard document".
static shared_ptr<IDoc> CreateCheckerboard3dDocument(bool use_spatial_index)
{
    const auto create_options = ClassFactory::CreateCreateOptionsUp();
    create_options->SetDocumentType(DocumentType::kImage3d);
    create_options->SetFilename(":memory:");
    //create_options->SetFilename("d:\\test.db");
    create_options->AddDimension('M');
    create_options->SetUseSpatialIndex(use_spatial_index);
    create_options->SetCreateBlobTable(true);

    auto doc = ClassFactory::CreateNew(create_options.get());
    const auto writer = doc->GetWriter3d();

    for (int column = 0; column < 10; ++column)
    {
        for (int row = 0; row < 10; ++row)
        {
            for (int z = 0; z < 10; ++z)
            {
                LogicalPositionInfo3D position_info;
                BrickBaseInfo brickInfo;
                TileCoordinate tc({ { 'M', z * 10 * 10 + column * 10 + row + 1 } });
                position_info.posX = column * 10;
                position_info.posY = row * 10;
                position_info.posZ = z * 10;
                position_info.width = 10;
                position_info.height = 10;
                position_info.depth = 10;
                position_info.pyrLvl = 0;
                brickInfo.pixelWidth = 10;
                brickInfo.pixelHeight = 10;
                brickInfo.pixelDepth = 10;
                brickInfo.pixelType = 0;
                writer->AddBrick(&tc, &position_info, &brickInfo, DataTypes::ZERO, TileDataStorageType::Invalid, nullptr);
            }
        }
    }

    return doc;
}

/// Utility for retrieving the M-coordinate from a list of tiles. No error handling is done here.
/// \param [in]     reader          The reader object.
/// \param          keys            The PKs of the tiles to query.
/// \returns        The m index of items.
static vector<int> GetMIndexOfItems(IDocRead3d* reader, const vector<dbIndex>& keys)
{
    vector<int> m_indices;
    for (const auto pk : keys)
    {
        TileCoordinate tc;
        reader->ReadBrickInfo(pk, &tc, nullptr, nullptr);
        int m_index;
        tc.TryGetCoordinate('M', &m_index);
        m_indices.push_back(m_index);
    }

    return m_indices;
}

TEST(Query3d, EmptyCoordinateQueryClauseCheckResult)
{
    // we query with an empty coordinate-query-clause, and expect that an empty clause means
    //  "no condition, all items are returned"
    const auto doc = CreateCheckerboard3dDocument(false);
    const auto reader = doc->GetReader3d();

    const CDimCoordinateQueryClause coordinate_query_clause;

    vector<dbIndex> result_indices;
    reader->Query(
        &coordinate_query_clause,
        nullptr,
        [&](dbIndex index)->bool
        {
            result_indices.emplace_back(index);
            return true;
        });

    // so, we expect to get all tiles in the document, and we check their correctness
    EXPECT_EQ(result_indices.size(), 1000ul);
    std::array<int, 1000> expected_result{};
    for (int i = 0; i < static_cast<int>(expected_result.size()); ++i)
    {
        expected_result[i] = 1 + i;
    }

    const auto m_indices = GetMIndexOfItems(reader.get(), result_indices);
    EXPECT_THAT(m_indices, UnorderedElementsAreArray(expected_result));
}

struct Query3dWithAndWithoutSpatialIndexFixture1 : public testing::TestWithParam<bool> {};
TEST_P(Query3dWithAndWithoutSpatialIndexFixture1, IndexQueryForCuboidAndCheckResult)
{
    // Using the 10x10x10 checkerboard-document, we query for tiles overlapping with the ROI (0,0,0,15,15,15).
   // We expect to find 8 tiles, with M=1, 2, 11, 12, 101, 102, 111, 112.
    const bool use_spatial_index = GetParam();
    const auto doc = CreateCheckerboard3dDocument(use_spatial_index);
    const auto reader = doc->GetReader3d();

    vector<dbIndex> result_indices;
    reader->GetTilesIntersectingCuboid(
        CuboidD{ 0, 0, 0, 15, 15 ,15 },
        nullptr,
        nullptr,
        [&](dbIndex index)->bool
        {
            result_indices.emplace_back(index);
            return true;
        });

    const auto m_indices = GetMIndexOfItems(reader.get(), result_indices);
    EXPECT_THAT(m_indices, UnorderedElementsAre(1, 11, 2, 12, 101, 102, 111, 112));
}

INSTANTIATE_TEST_SUITE_P(
    Query3d,
    Query3dWithAndWithoutSpatialIndexFixture1,
    testing::Values(true, false));

struct Query3dWithAndWithoutSpatialIndexFixture2 : public testing::TestWithParam<bool> {};
TEST_P(Query3dWithAndWithoutSpatialIndexFixture2, IndexQueryForCuboidAndCoordinateQueryAndCheckResult)
{
    // we use a combined "ROI and coordinate-query", we look for subblocks which intersect with the rectangle (0,0,0,15,15,15) and
    // with the M-index in the range 0 to 5 or 100 to 105 (exclusive the borders), i.e. (M > 0 and M < 5) or (M > 100 and M<105).
    // We expect to find four subblocks (with M-index 1, 2, 101 and 102).
    const bool use_spatial_index = GetParam();
    const auto doc = CreateCheckerboard3dDocument(use_spatial_index);
    const auto reader = doc->GetReader3d();

    CDimCoordinateQueryClause coordinate_query_clause;
    coordinate_query_clause.AddRangeClause('M', IDimCoordinateQueryClause::RangeClause{ 0, 5 });
    coordinate_query_clause.AddRangeClause('M', IDimCoordinateQueryClause::RangeClause{ 100, 105 });

    vector<dbIndex> result_indices;
    reader->GetTilesIntersectingCuboid(CuboidD{ 0, 0, 0, 15, 15, 15 },
        &coordinate_query_clause,
        nullptr,
        [&](dbIndex index)->bool
        {
            result_indices.emplace_back(index);
            return true;
        });

    const auto m_indices = GetMIndexOfItems(reader.get(), result_indices);
    EXPECT_THAT(m_indices, UnorderedElementsAre(1, 2, 101, 102));
}

INSTANTIATE_TEST_SUITE_P(
    Query3d,
    Query3dWithAndWithoutSpatialIndexFixture2,
    testing::Values(true, false));