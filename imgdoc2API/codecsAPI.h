// SPDX-FileCopyrightText: 2024 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

#pragma once

#include "imgdoc2API.h"
#include "importexport.h"
#include "errorcodes.h"
#include "bitmapinfointerop.h"
#include "decodedimageresultinterop.h"


EXTERNAL_API(ImgDoc2ErrorCode) DecodeImage(
                const BitmapInfoInterop* bitmap_info,
                std::uint8_t data_type,
                const void* compressed_data,
                std::uint64_t compressed_data_size,
                std::uint32_t destination_stride,
                AllocMemoryFunctionPointer allocate_memory_function,
                DecodedImageResultInterop* result,
                ImgDoc2ErrorInformation* error_information);

