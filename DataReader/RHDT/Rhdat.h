#pragma once
#include <string>

namespace rhdat{
    using std::string;

    /**
     * @brief RHDT file format. Equivalent of R H D T
     */
    constexpr int RHDAT_MAGIC_BYTES='TDHR';
    static_assert(RHDAT_MAGIC_BYTES==1413761106,"Your compiler does not support multi-character constants. Replace characters with equivalent value.");
    
    /**
     * @brief Current version of RHDAT file format.
     */
    constexpr int RHDAT_CURRENT_VERSION=1;
    typedef int Version;

    enum class EntryType: int{
        Units=1
    };

    struct EntryPtr
    {
        EntryType entryType;
        unsigned int entryCount;
        unsigned int dataPtr;
    };

    /**
     * @brief DataReader custom RHDAT filetype header.
     */
    struct RhdatHeader
    {
        int magicBytes;

        /**
         * @brief Version of filetype.
         */
        Version version;

        /**
         * @brief Entry count.
         */
        unsigned int ptrConunt;
        // ptrs below
    };
}