#include "GameObject.h"

GameObject::GameObject(ID objectID):objectID(objectID){}

ID GameObject::getID()
{
    return objectID;
}