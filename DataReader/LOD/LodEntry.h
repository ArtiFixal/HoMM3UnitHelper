#pragma once
#include <inttypes.h>

namespace h3lod{

    /**
     * @brief Describes @code LodEntry file type.
     */
    typedef enum class FileType_t: uint32_t{
        H3M=1,
        TXT=2,
        PCX=16,
        BACKGROUNG_PCX=17,
        SPELL_DEF=64,
        SPRITE_DEF=65,
        CREATURE_DEF=66,
        MAP_OBJECT_DEF=67,
        MAP_HERO_DEF=68,
        TERRAIN_DEF=69,
        CURSOR_DEF=70,
        INTERFACE_DEF=71,
        SPRITE_FRAME=72,
        COMBAT_HERO_DEF=73,
        MSK=79,
        FNT=80,
        PAL=96,
        LOD=200,
        EXTENSION_LOD=500,
        UNKNOWN=0xFFFFFFFF
    } FileType;

    /**
     * @brief 32 byte header entry describing single file inside .LOD archive.
     */
    struct LodEntry
    {
        char fileName[16];

        /**
         * @brief Points where file starts in .LOD archive.
         */
        uint32_t filePointer;
        uint32_t uncompressedSize;
        FileType fileType;
        uint32_t compressedSize;
    };
}