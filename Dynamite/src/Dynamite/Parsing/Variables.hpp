#pragma once

#include "Dynamite/Tokens/Token.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Types
	/////////////////////////////////////////////////////////////////
	// TODO: Add pointers somehow
	enum class BaseType : uint8_t
	{
		None = 0,

		Void = (uint8_t)TokenType::Void,

		Bool = (uint8_t)TokenType::Bool,

		Int8 = (uint8_t)TokenType::Int8,
		Int16 = (uint8_t)TokenType::Int16,
		Int32 = (uint8_t)TokenType::Int32,
		Int64 = (uint8_t)TokenType::Int64,

		UInt8 = (uint8_t)TokenType::UInt8,
		UInt16 = (uint8_t)TokenType::UInt16,
		UInt32 = (uint8_t)TokenType::UInt32,
		UInt64 = (uint8_t)TokenType::UInt64,

		Float32 = (uint8_t)TokenType::Float32,
		Float64 = (uint8_t)TokenType::Float64,

		Char = (uint8_t)TokenType::Char,
		String = (uint8_t)TokenType::String,

		Pointer
	};

	// Note: Allocate next with 'new' and the destructor will 'delete' it for you.
	struct ValueType
	{
	public:
		BaseType Base;

		// Will have a next if base is a pointer
		// else its a nullptr.
		// TODO: Add a better way of storing than pointers
		ValueType* Next;

	public:
		ValueType(BaseType base = BaseType::None, ValueType* next = nullptr);
		~ValueType();

	public:
		bool operator == (BaseType otherBase) const;
		bool operator != (BaseType otherBase) const;

		bool operator == (const ValueType& other) const;
		bool operator != (const ValueType& other) const;
	};

	struct Variable
	{
	public:
		std::string Name = {};
		ValueType Type = {};
	};

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string ValueTypeToStr(const ValueType& type);
	size_t ValueTypeSize(const ValueType& type);
	bool ValueTypeCastable(const ValueType& from, const ValueType& to);
	std::string ValueTypeCast(const ValueType& from, const ValueType& to, const std::string& value, bool* dataLostPtr = nullptr);

	BaseType GetBaseType(TokenType literalType, const std::string& value);

}
