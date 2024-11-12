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

		Mut = 1 << 0, 
		Volatile = 1 << 1,

		Pointer = 1 << 2,
		Reference = 1 << 3,

		Array = 1 << 4,
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

	struct QualifierGroup
	{
	public:
		TypeQualifier Qualifiers = TypeQualifier::None;
		size_t ArraySize = 0;

	public:
		// Constructors
		QualifierGroup() = default;
		QualifierGroup(TypeQualifier qualifiers, size_t arraySize = 0);

		// Operators
		bool operator == (const QualifierGroup& other) const;
		bool operator != (const QualifierGroup& other) const;

	public:
		// Functions
		void Add(TypeQualifier qualifier);

		// Qualifier checks
		bool Contains(TypeQualifier qualifier) const;

		bool IsMut() const;
		bool IsVolatile() const;

		bool IsPointer() const;
		bool IsReference() const;

		bool IsArray() const;

		// Array functions
		inline size_t GetArraySize() const { return ArraySize; }
		inline void SetArraySize(size_t size) { ArraySize = size; }

		// Other
		// Note: Splits the single TypeQualifier value into all typequalifiers seperately.
		std::vector<TypeQualifier> SplitQualifiers() const;
	};

	/////////////////////////////////////////////////////////////////
	// Complete Type
	/////////////////////////////////////////////////////////////////
	struct Type
	{
	public:
		// Ex. mut, volatile
		QualifierGroup FrontQualifiers;

		TypeInfo Information = {};
		
		// Ex. *, &, []
		std::vector<QualifierGroup> BackQualifiers = { };

	public:
		// Constructors
		Type() = default;
		Type(const TypeInfo& info);
		Type(QualifierGroup front, const TypeInfo& info, const std::vector<QualifierGroup>& back = {});

		// Operators
		bool operator == (const Type& other) const;
		bool operator != (const Type& other) const;

		bool operator == (const TypeInfo& info) const;
		bool operator != (const TypeInfo& info) const;

		// Checks for the latest back qualifiers
		bool IsMut() const;
		bool IsVolatile() const;

		bool IsPointer() const;
		bool IsReference() const;

		bool IsArray() const;

		bool IsUnsigned() const;
		bool IsSigned() const;

		// Type checks
		bool IsVoid() const;

		bool IsBool() const;
		bool IsIntegral() const;
		bool IsFloat() const;
		bool IsChar() const;

		// Array
		size_t GetArraySize() const;
		void SetArraySize(size_t size);

		// Modifying
		void AddToBack(TypeQualifier qualifier, Optional<size_t> arraySize = {});
		void PushBack(TypeQualifier qualifier, size_t arraySize);
		void PopBack();

		// Utils
		Type RemoveReference() const;
		Type RemovePointer() const;

		// Note: Removes front mut & volatile.
		Type Clean() const;
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeSpecifierToString(TypeSpecifier specifier);
	std::string TypeQualifierToString(TypeQualifier qualifier, size_t arraySize = 0);

}