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

		//Reference,
		Pointer,

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
		bool operator == (const TypeInfo& other) const;
		bool operator != (const TypeInfo& other) const;
	};

	/////////////////////////////////////////////////////////////////
	// Complete Type
	/////////////////////////////////////////////////////////////////
	struct Type
	{
	public:
		struct QualifierPack // Note: We have this struct to support array sizes
		{
		public:
			TypeQualifier Qualifier;
			std::string Value; // ex. Array size, so '5'

		public:
			bool operator == (const TypeQualifier& qualifier) const;
			bool operator != (const TypeQualifier& qualifier) const;

			bool operator == (const QualifierPack& other) const;
			bool operator != (const QualifierPack& other) const;

			inline operator TypeQualifier() const { return Qualifier; }
		};
	public:
		// Ex. mut, volatile
		std::vector<QualifierPack> FrontQualifiers = { };

		TypeInfo Information = {};
		
		// Ex. *, &, []
		std::vector<QualifierPack> BackQualifiers = { };

	public:
		// Constructors
		Type() = default;
		Type(const TypeInfo& info);
		Type(const std::vector<QualifierPack>& front, const TypeInfo& info, const std::vector<QualifierPack>& back = {});
		~Type() = default;

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

		// Adds
		void AddMut();
		void AddVolatile();

		void AddPointer();

		void AddArray(const std::string& size);

		std::string GetArraySize() const;
		void SetArraySize(const std::string& value);

		// Note: Removes mut & volatile, but not pointers.
		Type Clean() const;
		Type Copy() const;
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string TypeSpecifierToString(TypeSpecifier specifier);
	std::string TypeQualifierToString(TypeQualifier qualifier, const std::string& value = {});

	TypeSpecifier TokenTypeToTypeSpecifier(TokenType tokenType);
	TypeQualifier TokenTypeToTypeQualifier(TokenType tokenType);

}