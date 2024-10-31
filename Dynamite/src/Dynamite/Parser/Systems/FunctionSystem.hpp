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
		public:
			struct Overload
			{
			public:
				Type ReturnType;
				std::vector<std::pair<Type, bool>> Parameters; // Type & required bool

			public:
				bool operator == (const Overload& other);
				bool operator != (const Overload& other);
			};
		public:
			std::string Name;
			std::vector<Overload> Overloads;
		};
	public:
		static void Reset();

		static void Add(const std::string& name, Type returnType, const std::vector<std::pair<Type, bool>>& parameters = { });

		static std::vector<Type> GetReturnTypes(const std::string& functionName);
		static std::vector<Type> GetArgumentTypes(const std::string& functionName, size_t index);

		static std::vector<size_t> GetArgCounts(const std::string& functionName);
		static std::vector<size_t> GetRequiredArgCounts(const std::string& functionName);

		static bool Exists(const std::string& name);
		static bool Exists(const std::string& name, const Type& returnType, const std::vector<Type>& parameters = { });

		static Function* GetFunction(const std::string& name);
		static Function::Overload* GetOverload(const std::string& name, size_t overloadIndex = 0);

	private:
		inline static std::vector<Function> s_Functions = {};
	};

}