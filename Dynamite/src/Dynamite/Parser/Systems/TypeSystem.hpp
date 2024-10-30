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
	class TypeSystem // Note: All systems are static for debug/printing purposes
	{
	public:
		// Main functions
		static void Reset();

		static bool Castable(const Type& from, const Type& to);
		static bool Cast(const Type& from, const Type& to, std::string& value);

		static Type GetTypeFromLiteral(TokenType literalType, const std::string& value);
		static Type GetBinExprResultType(const Type& lhs, TokenType operation, const Type& rhs);

		static std::string ToString(const Type& type);

		static bool Exists(const std::string& name);

	public:
		// Check functions
		static bool IsVoid(const Type& type);

		static bool IsBool(const Type& type);
		static bool IsIntegral(const Type& type);
		static bool IsFloat(const Type& type);
		static bool IsChar(const Type& type);

	public:
		// Get functions
		static size_t GetSize(const Type& type);

		static Type GetLargest(const Type& lhs, const Type& rhs);

		static Type GetInt(size_t size = GetSize(Type(TypeSpecifier::Int32)));
		static Type GetUInt(size_t size = GetSize(Type(TypeSpecifier::UInt32)));
		static Type GetFloat(size_t size = GetSize(Type(TypeSpecifier::Float32)));

	private:
		// Note: We have this class so we can support custom
		// types in the future
	};

}