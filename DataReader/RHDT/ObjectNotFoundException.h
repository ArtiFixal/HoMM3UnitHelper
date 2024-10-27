#pragma once
#include <stdexcept>

class ObjectNotFoundException: public std::runtime_error
{
private:
    
public:
    ObjectNotFoundException();
    ~ObjectNotFoundException();
};