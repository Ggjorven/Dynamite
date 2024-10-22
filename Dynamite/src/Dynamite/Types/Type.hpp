#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Tokens/Token.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Enums
	/////////////////////////////////////////////////////////////////
	enum class TypeSpecifier : uint8_t
	{
		None = 0,

		Void,

		Bool,

		Int8,
		Int16,
		Int32,
		Int64,

		UInt8,
		UInt16,
		UInt32,
		UInt64,

		Float32,
		Float64,

		Char,
		String,

		// Note: A custom class or type is also an identifier.
		Identifier
	};

	enum class TypeQualifier : uint8_t
	{
		None = 0,

		Const, 
		Volatile,

		Pointer,
		Reference,
	};

	/////////////////////////////////////////////////////////////////
	// Type Information
	/////////////////////////////////////////////////////////////////
	struct TypeInfo
	{
	public:
		TypeSpecifier Specifier;

		// Note: Check if has value with .empty(), it will
		// only contain a value when Specifier == TypeSpecifier::Identifier
		std::string Value;

	public:
		TypeInfo(TypeSpecifier specifier = TypeSpecifier::None, const std::string& value = {});
	};

	/////////////////////////////////////////////////////////////////
	// Complete Type
	/////////////////////////////////////////////////////////////////
	struct Type
	{
	public:
		// Ex. const, volatile, *, &
		std::vector<TypeQualifier> FrontQualifiers = { };

		TypeInfo Information = {};
		
		// Ex. const, volatile, *, &
		std::vector<TypeQualifier> BackQualifiers = { };
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeQualifierToString(TypeQualifier qualifier);
	std::string TypeSpecifierToString(TypeSpecifier specifier);
	std::string TypeToString(const Type& type);

}