#include "TxtReader.h"

namespace h3lod{

    template <class T>
    TxtReader<T>::TxtReader(h3lod::LodFile& fileToRead): h3lod::GameDataReader<T>(fileToRead), currentLine(1)
    {
        //this->reader=new std::istringstream(fileToRead.getData());
        if(findCharInString(this->file.getData(),'\t',0,32)!=string::npos)
        {
            fileDataType=DataType::DATA_LIST;
            goToLine(2);
        }
        else
            fileDataType=DataType::STRING_LIST;
        fileBegin=this->reader.tellg();
    }

    template <typename T>
    size_t TxtReader<T>::findCharInString(string &str, char c, size_t pos, size_t maxPos)
    {
        for(int i=0;i<maxPos;i++)
        {
            if(str.at(i)==c)
                return i;
        }
        return string::npos;
    }

    template <class T>
    void TxtReader<T>::skipChar(char delimiter)
    {
        this->reader.ignore(std::numeric_limits<std::streamsize>::max(),delimiter);
    }

    template <class T>
    void TxtReader<T>::skipNChars(char delimiter,int n){
        while(n--)
            skipChar(delimiter);
    }

    template <class T>
    void TxtReader<T>::skipLine()
    {
        this->reader.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        currentLine++;
    }

    template <class T>
    size_t TxtReader<T>::getCurrentLineNum()
    {
        return currentLine;
    }

    template <class T>
    void TxtReader<T>::goToLine(size_t line)
    {
        currentLine=1;
        this->reader.seekg(fileBegin);
        while(line--)
            skipLine();
    }

    template <class T>
    string TxtReader<T>::readLine()
    {
        string line;
        std::getline(this->reader,line);
        currentLine++;
        return line;
    }

    template <class T>
    inline std::vector<string> TxtReader<T>::readStrings()
    {
        return readStrings('\t');
    }

    template <class T>
    std::vector<string> TxtReader<T>::readStrings(char delimiter)
    {
        string line=readLine();
        std::vector<string> stringsRead;
        stringsRead.reserve(25);
        std::istringstream tokenizer=std::istringstream(line,std::ios_base::in);
        string str;
        while(std::getline(tokenizer,str,delimiter))
            stringsRead.push_back(str);
        return stringsRead;
    }

    template class TxtReader<UnitExtended>;
}