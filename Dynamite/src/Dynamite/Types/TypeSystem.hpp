#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// TypeSystem
	/////////////////////////////////////////////////////////////////
	class TypeSystem
	{
	public:
		static void Reset();

		static bool Castable(const Type& from, const Type& to);
		static bool Cast(const Type& from, const Type& to, std::string& value);

		static Type GetTypeFromLiteral(TokenType literalType, const std::string& value);
		static Type GetResultType(const Type& lhs, const Type& rhs);

		static std::string ToString(const Type& type);

		static bool Exists(const std::string& name);

	private:
		// Note: We have this class so we can support custom
		// types in the future
	};

}