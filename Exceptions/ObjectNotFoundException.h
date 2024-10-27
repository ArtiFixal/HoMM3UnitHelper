#pragma once
#include <stdexcept>
#include <variant>
#include "GameObject.h"

class ObjectNotFoundException: public std::exception
{
private:
    typedef std::variant<ID,std::string> Identifier;
    Identifier identifier;
    std::string msg;

public:
    ObjectNotFoundException(Identifier objectID): ObjectNotFoundException("Object",objectID){};
    ObjectNotFoundException(std::string objectType,Identifier objectID):std::exception(),identifier(objectID),msg(objectType+" identified by: "+(objectID.index()==0?std::to_string(std::get<ID>(objectID)):std::get<std::string>(objectID))+" not found"){};
    ~ObjectNotFoundException(){};

    const char* what() const noexcept{
        return msg.c_str();
    }

    Identifier getObjectIdentifier(){
        return identifier;
    }
};