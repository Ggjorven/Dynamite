#pragma once

#include "Dynamite/Tokens/Token.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Types
	/////////////////////////////////////////////////////////////////
	enum class ValueType : uint16_t
	{
		None = 0,

		Bool = TokenType::Bool,

		Int8 = TokenType::Int8,
		Int16 = TokenType::Int16,
		Int32 = TokenType::Int32,
		Int64 = TokenType::Int64,

		UInt8 = TokenType::UInt8,
		UInt16 = TokenType::UInt16,
		UInt32 = TokenType::UInt32,
		UInt64 = TokenType::UInt64,

		Float32 = TokenType::Float32,
		Float64 = TokenType::Float64,

		Char = TokenType::Char,
		String = TokenType::String,
	};

	/////////////////////////////////////////////////////////////////
	// Conversion of Types
	/////////////////////////////////////////////////////////////////
	template<ValueType T> struct ValueTypeToCTypeImpl;

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
	std::string ValueTypeToASM(ValueType type);
	std::string ValueTypeToStr(ValueType type);
	bool ValueTypeCastable(ValueType from, ValueType to);
	std::string ValueTypeCast(ValueType from, ValueType to, const std::string& value, bool* dataLostPtr = nullptr);

}