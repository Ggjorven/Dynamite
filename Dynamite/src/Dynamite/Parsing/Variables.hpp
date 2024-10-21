#pragma once

#include "Dynamite/Tokens/Token.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Types
	/////////////////////////////////////////////////////////////////
	// TODO: Add pointers somehow
	enum class ValueType : uint8_t
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
	};

	struct Variable
	{
	public:
		std::string Name = {};
		ValueType Type = ValueType::None;
	};

	/////////////////////////////////////////////////////////////////
	// Conversion of Types
	/////////////////////////////////////////////////////////////////
	template<ValueType T> struct ValueTypeToCTypeImpl;

	template<> struct ValueTypeToCTypeImpl<ValueType::Void> { using Type = void; };

	template<> struct ValueTypeToCTypeImpl<ValueType::Bool> { using Type = bool; };

	template<> struct ValueTypeToCTypeImpl<ValueType::Int8> { using Type = int8_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Int16> { using Type = int16_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Int32> { using Type = int32_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Int64> { using Type = int64_t; };

	template<> struct ValueTypeToCTypeImpl<ValueType::UInt8> { using Type = uint8_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::UInt16> { using Type = uint16_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::UInt32> { using Type = uint32_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::UInt64> { using Type = uint64_t; };

	template<> struct ValueTypeToCTypeImpl<ValueType::Float32> { using Type = float; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Float64> { using Type = double; };

	template<> struct ValueTypeToCTypeImpl<ValueType::Char> { using Type = char; };

	// Note: String doesn't have a C(++) equivalent at the moment.

	template<ValueType T>
	using ValueTypeToCType = ValueTypeToCTypeImpl<T>::Type;

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string ValueTypeToStr(ValueType type);
	size_t ValueTypeSize(ValueType type);
	bool ValueTypeCastable(ValueType from, ValueType to);
	std::string ValueTypeCast(ValueType from, ValueType to, const std::string& value, bool* dataLostPtr = nullptr);

	ValueType GetValueType(TokenType literalType, const std::string& value);

}
