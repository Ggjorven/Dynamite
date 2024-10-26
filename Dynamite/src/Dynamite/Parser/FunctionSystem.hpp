#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// FunctionSystem
	/////////////////////////////////////////////////////////////////
	class FunctionSystem
	{
	public:
		struct Function
		{
			std::string Name;
			Type ReturnType;
			std::vector<Type> Parameters;
		};
	public:
		void Reset();

		void Add(const std::string& name, Type returnType, const std::vector<Type>& parameters = { });

		Optional<Type> GetReturnType(const std::string& functionName);
		Optional<Type> GetArgumentType(const std::string& functionName, size_t index);

	private:
		std::vector<Function> m_Functions = {};
	};

}