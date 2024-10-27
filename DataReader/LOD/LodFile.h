#pragma once
#include <string>
#include <zlib.h>
#include "LodEntry.h"
#include "h3lod_defs.h"
#include <limits>

namespace h3lod{

    /**
     * @brief File placed inside .LOD archive. Stores data in-memory.
     */
    class LodFile
    {
    private:
        LodEntry fileEntry;

        /**
         * Uncompressed file data.
         */
        string data;

    public:
        LodFile(LodEntry fileEntry, string& compressedData);
        ~LodFile(){};

        string& getData();
        LodEntry getFileEntry();
    };
}
