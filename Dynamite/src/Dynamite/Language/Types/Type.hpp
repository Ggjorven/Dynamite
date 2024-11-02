#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite::Language
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

		Array 
	};

	/////////////////////////////////////////////////////////////////
	// Information structs
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

	struct QualifierInfo
	{
	public:
		TypeQualifier Qualifier;

		// Note: Check if has value with .empty(), it will
		// only contain a value when Qualifier == TypeQualifier::Array
		std::string Value; // ex. Array size, so '5'

	public:
		// Constructors
		QualifierInfo(TypeQualifier qualifier = TypeQualifier::None, const std::string& value = {});

		// Operators
		bool operator == (const TypeQualifier& qualifier) const;
		bool operator != (const TypeQualifier& qualifier) const;

		bool operator == (const QualifierInfo& other) const;
		bool operator != (const QualifierInfo& other) const;
	};

	/////////////////////////////////////////////////////////////////
	// Complete Type
	/////////////////////////////////////////////////////////////////
	struct Type
	{
	public:
		// Ex. mut, volatile
		std::vector<QualifierInfo> FrontQualifiers = { };

		TypeInfo Information = {};
		
		// Ex. *, &, []
		std::vector<QualifierInfo> BackQualifiers = { };

	public:
		// Constructors
		Type() = default;
		Type(const TypeInfo& info);
		Type(const std::vector<QualifierInfo>& front, const TypeInfo& info, const std::vector<QualifierInfo>& back = {});

		// Operators
		bool operator == (const Type& other) const;
		bool operator != (const Type& other) const;

		bool operator == (const TypeInfo& info) const;
		bool operator != (const TypeInfo& info) const;

		// Checks
		bool IsMut() const;
		bool IsVolatile() const;

		bool IsPointer() const;

		bool IsArray() const;

		bool IsUnsigned() const;

		// Type checks
		bool IsVoid() const;

		bool IsBool() const;
		bool IsIntegral() const;
		bool IsFloat() const;
		bool IsChar() const;

		// Adds
		void AddMut();
		void AddVolatile();

		void AddPointer();

		void AddArray(const std::string& size);

		std::string GetArraySize() const;
		void SetArraySize(const std::string& value);

		// Utils
		// Note: Removes mut & volatile, but not pointers.
		Type Clean() const;
		Type Copy() const;
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeSpecifierToString(TypeSpecifier specifier);
	std::string TypeQualifierToString(TypeQualifier qualifier, const std::string& value = {});

}