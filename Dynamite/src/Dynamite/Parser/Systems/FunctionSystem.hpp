#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	class FunctionSystem // Note: All systems are static for debug/printing purposes
	{
	public:
		struct Function
		{
			std::string Name;
			Type ReturnType;
			std::vector<std::pair<Type, bool>> Parameters; // Type & required bool
		};
	public:
		static void Reset();

		static void Add(const std::string& name, Type returnType, const std::vector<std::pair<Type, bool>>& parameters = { });

		static Optional<Type> GetReturnType(const std::string& functionName);
		static Optional<Type> GetArgumentType(const std::string& functionName, size_t index);

		static size_t GetArgCount(const std::string& functionName);
		static size_t GetRequiredArgCount(const std::string& functionName);

	private:
		inline static std::vector<Function> s_Functions = {};
	};

}