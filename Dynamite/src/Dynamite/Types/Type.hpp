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

		Array
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
		// Constructors
		TypeInfo(TypeSpecifier specifier = TypeSpecifier::None, const std::string& value = {});

		// Operators
		bool operator == (const TypeInfo& other);
		bool operator != (const TypeInfo& other);
	};

	/////////////////////////////////////////////////////////////////
	// Complete Type
	/////////////////////////////////////////////////////////////////
	struct Type
	{
	public:
		// Ex. const, volatile
		std::vector<TypeQualifier> FrontQualifiers = { };

		TypeInfo Information = {};
		
		// Ex. *, &
		std::vector<TypeQualifier> BackQualifiers = { };

	public:
		// Constructors
		Type() = default;
		Type(const TypeInfo& info);
		Type(const std::vector<TypeQualifier>& front, const TypeInfo& info, const std::vector<TypeQualifier>& back = {});
		~Type() = default;

		// Operators
		bool operator == (const Type& other);
		bool operator != (const Type& other);

		bool operator == (const TypeInfo& info);
		bool operator != (const TypeInfo& info);

		// Methods
		bool IsConst();
		bool IsVolatile();

		bool IsPointer();
		bool IsReference();

		bool IsArray();

		// Note: Removes const, volatile & references, but not pointers.
		Type Clean();
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeSpecifierToString(TypeSpecifier specifier);
	std::string TypeQualifierToString(TypeQualifier qualifier);
	std::string TypeToString(const Type& type);

}