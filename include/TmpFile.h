#ifndef TMP_FILE_H
#define TMP_FILE_H
#include <fstream>
#include <filesystem>

/**
 * @brief Temporary file with fstream bidirectional I/O.
 */
class TmpFile: public std::fstream
{
private:
    std::filesystem::path& filePath;

public:
    TmpFile(std::filesystem::path& filePath,std::ios_base::openmode openMode);
    ~TmpFile();

    std::filesystem::path& getFilePath();
};
#endif
