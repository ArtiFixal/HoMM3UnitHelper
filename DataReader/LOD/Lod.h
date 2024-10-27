#pragma once
#include "h3lod_defs.h"
#include "LodEntry.h"

namespace h3lod{

    // LOD file format. Equivalent of L O D \0
    constexpr int LOD_FILE_MAGIC_BYTES='\0DOL';
    static_assert(LOD_FILE_MAGIC_BYTES==4476748,"Your compiler does not support multi-character constants. Replace characters with equivalent value.");
    
    /**
     * @brief .LOD archive header.
     */
    struct LodHeader
    {
        int magicBytes;
        FileType fileType;
        uint fileCount;
    };
}
