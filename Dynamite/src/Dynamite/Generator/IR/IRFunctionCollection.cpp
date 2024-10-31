#include "dypch.h"
#include "IRFunctionCollection.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

#include "Dynamite/Generator/Types/GeneratorTypes.hpp"

namespace Dynamite
{

    /////////////////////////////////////////////////////////////////
    // Main functions
    /////////////////////////////////////////////////////////////////
    void IRFunctionCollection::Reset()
    {
        s_Functions.clear();
    }

    void IRFunctionCollection::AddFunction(const std::string& name, const Function& func)
    {
        s_Functions[name].push_back(func);
    }

    llvm::FunctionCallee& IRFunctionCollection::GetLLVMFunction(const std::string& name, size_t overloadIndex)
    {
        return s_Functions.at(name)[overloadIndex].Callee;
    }

    llvm::Value* IRFunctionCollection::GetLLVMArg(const std::string& funcName, const std::string& argName, size_t overloadIndex)
    {
        return s_Functions.at(funcName)[overloadIndex].Arguments.at(argName);
    }

}