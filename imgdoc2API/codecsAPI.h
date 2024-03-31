#pragma once

#include "imgdoc2API.h"
#include "importexport.h"
#include "errorcodes.h"
#include "bitmapinfointerop.h"
#include "decodeimageresultinterop.h"


EXTERNAL_API(ImgDoc2ErrorCode) DecodeImageJpgXr(
                const BitmapInfoInterop* bitmap_info,
                const void* compressed_data,
                std::uint64_t compressed_data_size,
                std::uint32_t destination_stride,
                AllocMemoryFunctionPointer allocate_memory_function,
                DecodeImageResultInterop* result,
                ImgDoc2ErrorInformation* error_information);

