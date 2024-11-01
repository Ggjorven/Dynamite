#include "dypch.h"
#include "IRFunctionCollection.hpp"

#include "Core/Logging.hpp"

#include "Compiler/Compiler.hpp"

namespace Dynamite::Language
{

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    void IRFunctionCollection::Reset()
    {
        s_Functions.clear();
    }

    void IRFunctionCollection::AddFunction(const std::string& name, const GeneratorFunction& func)
    {
        s_Functions[name].push_back(func);
    }

    GeneratorFunction& IRFunctionCollection::GetFunction(const std::string& name, size_t overloadIndex)
    {
        return s_Functions.at(name)[overloadIndex];
    }

}