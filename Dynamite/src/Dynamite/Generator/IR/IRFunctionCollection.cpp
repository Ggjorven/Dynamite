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

    IRFunctionCollection::Function& IRFunctionCollection::GetFunction(const std::string& name, size_t overloadIndex)
    {
        return s_Functions.at(name)[overloadIndex];
    }

    llvm::FunctionCallee& IRFunctionCollection::GetLLVMFunction(const std::string& name, size_t overloadIndex)
    {
        return s_Functions.at(name)[overloadIndex].Callee;
    }

    llvm::Value* IRFunctionCollection::GetLLVMArg(const std::string& funcName, size_t argIndex, size_t overloadIndex)
    {
        return s_Functions.at(funcName)[overloadIndex].Arguments.at(argIndex).first;
    }

    Type& IRFunctionCollection::GetLLVMArgType(const std::string& funcName, size_t argIndex, size_t overloadIndex)
    {
        return s_Functions.at(funcName)[overloadIndex].Arguments.at(argIndex).second;
    }

}