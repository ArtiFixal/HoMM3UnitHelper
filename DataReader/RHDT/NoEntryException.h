#pragma once
#include <stdexcept>
#include "Rhdat.h"

namespace rhdat{

    class NoEntryException: public std::runtime_error
    {
    private:
        
    public:
        NoEntryException(): std::runtime_error("Writer doesn't have any entries yet"){};
        NoEntryException(EntryType entry): std::runtime_error("Writer doesn't have given entry: "+static_cast<int>(entry)){};
        ~NoEntryException(){};
    };
}