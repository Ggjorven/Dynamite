#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/LiteralType.hpp"
#include "Dynamite/Language/OperationType.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// TypeCollection
	/////////////////////////////////////////////////////////////////
	class TypeCollection
	{
	public:
		// Main functions
		static void Reset();

		static bool ImplicitCastable(const Type& from, const Type& to);
		static bool ExplicitCastable(const Type& from, const Type& to);

		// Main get type functions
		static Type GetTypeFromLiteral(LiteralType literalType, const std::string& value);
		static Type GetBinExprResultType(const Type& lhs, OperationType operation, const Type& rhs);

		static bool Exists(const std::string& name);

	public:
		// Get functions
		static size_t GetSize(const Type& type);

		static Type GetLargest(const Type& lhs, const Type& rhs);

		static Type GetInt(size_t size = GetSize(Type(TypeSpecifier::Int32)));
		static Type GetUInt(size_t size = GetSize(Type(TypeSpecifier::UInt32)));
		static Type GetFloat(size_t size = GetSize(Type(TypeSpecifier::Float32)));

	public:
		// Debug functions
		static std::string ToString(const Type& type);

	private:
		// Note: We have this class so we can support custom
		// types in the future
	};

}