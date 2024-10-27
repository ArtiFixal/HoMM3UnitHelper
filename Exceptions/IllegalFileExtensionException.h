#pragma once
#include <stdexcept>
#include <string>

class IllegalFileExtensionException: public std::runtime_error
{
private:
    /**
     * Required extension.
     */
    const char* expected;

    /**
     * Actual extension.
     */
    const char* found;
public:
    IllegalFileExtensionException(const char* expected,const char* found): std::runtime_error(std::string("Illegal file extension. Expected: ").append(expected).append(" but found: ").append(found)), expected(expected),found(found){};
    

    /**
     * @param expected Expected extension magic bytes.
     * @param found Actual extension magic bytes.
     */
    IllegalFileExtensionException(int expected,int found): IllegalFileExtensionException(reinterpret_cast<char*>(expected),reinterpret_cast<char*>(found)){};
    ~IllegalFileExtensionException(){};

    const char* getExpectedExtension(){
        return expected;
    }

    const char* getFoundExtension(){
        return found;
    }
};