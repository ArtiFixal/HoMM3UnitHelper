#pragma once
#include <string>

namespace hdat{
    using std::string;

    /**
     * @brief HDAT file format. Equivalent of H D A T
     */
    constexpr int HDAT_MAGIC_BYTES='TADH';
    static_assert(HDAT_MAGIC_BYTES==1413563464,"Your compiler does not support multi-character constants. Replace characters with equivalent value.");

    /**
     * @brief HotA data file format header.
     */
    struct HdatHeader
    {
        int magicBytes;
        unsigned int fileVersion;
        unsigned int entryCount;
    };
}