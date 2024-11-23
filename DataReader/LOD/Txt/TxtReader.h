#pragma once
#include <string>
#include <sstream>
#include <limits>
#include <vector>
#include "LodFile.h"
#include "GameDataReader.h"
#include "UnitExtended.h"

namespace h3lod{

    /**
     * @brief Reads data from TXT files inside .LOD archive.
     * 
     * @tparam T Data type stored in this TXT file.
     */
    template<typename T>
    class TxtReader: public GameDataReader<T>
    {
        private:
            typedef enum class DataType_t: int{
                STRING_LIST,DATA_LIST
            } DataType;

            size_t findCharInString(string& str,char c,size_t pos,size_t maxPos);

            DataType fileDataType;
            size_t currentLine;
        
        protected:
            std::streampos fileBegin;
            
            /**
             * @brief Skips input characters until given character is met including 
             * that character.
             * 
             * @param delimiter Which character to skip to.
             */
            void skipChar(char delimiter);

            /**
             * @brief Skips input characters until n given character were skipped.
             * 
             * @param delimiter Which characters to skip.
             * @param n How many times to skip given character.
             */
            void skipNChars(char delimiter,int n);

        public:
            TxtReader(LodFile& fileToRead);
            ~TxtReader(){};

            void skipLine();
            size_t getCurrentLineNum();

            /**
             * @brief Sets position to a given line.
             * 
             * @param line Where to set the position.
             */
            void goToLine(size_t line);
            string readLine();

            /**
             * @brief Reads single line and splits it into separate strings using
             * '\t' delimiter.
             * 
             * @return Vector of read strings.
             */
            std::vector<string> readStrings();

            /**
             * @brief Reads single line and splits it into separate string using
             * given delimiter.
             * 
             * @param delimiter By what to split.
             * 
             * @return Vector of read strings.
             */
            std::vector<string> readStrings(char delimiter);
    };
}