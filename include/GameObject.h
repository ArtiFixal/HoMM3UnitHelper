#pragma once

typedef unsigned int ID;

class GameObject
{
protected:
    ID objectID;

public:
    GameObject(ID objectID);
    ~GameObject(){};

    ID getID();
};