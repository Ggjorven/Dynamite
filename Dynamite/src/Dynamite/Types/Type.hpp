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

		Mut, 
		Volatile,

		Pointer,
		Reference,

		Array // TODO: Add array sizes
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
		bool operator == (const TypeInfo& other) const;
		bool operator != (const TypeInfo& other) const;
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
		bool operator == (const Type& other) const;
		bool operator != (const Type& other) const;

		bool operator == (const TypeInfo& info) const;
		bool operator != (const TypeInfo& info) const;

		// Methods
		TypeQualifier CleanBackQualifier() const; // Returns the most back qualifier (removes first reference if found)

		bool IsMut() const;
		bool IsVolatile() const;

		bool IsPointer() const;
		bool IsReference() const;

		bool IsArray() const;

		// Note: Removes const, volatile & references, but not pointers.
		Type Clean() const;
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeSpecifierToString(TypeSpecifier specifier);
	std::string TypeQualifierToString(TypeQualifier qualifier);

	TypeSpecifier TokenTypeToTypeSpecifier(TokenType tokenType);
	TypeQualifier TokenTypeToTypeQualifier(TokenType tokenType);

}