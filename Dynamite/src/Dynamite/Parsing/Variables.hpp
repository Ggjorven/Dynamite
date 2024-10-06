#pragma once

#include "Dynamite/Tokens/Token.hpp"

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Types
	/////////////////////////////////////////////////////////////////
	enum class ValueType : uint8_t
	{
		None = 0,

		Int8 = TokenType::Int8,
		Int16 = TokenType::Int16,
		Int32 = TokenType::Int32,
		Int64 = TokenType::Int64,

		UInt8 = TokenType::UInt8,
		UInt16 = TokenType::UInt16,
		UInt32 = TokenType::UInt32,
		UInt64 = TokenType::UInt64,
	};

	/////////////////////////////////////////////////////////////////
	// Conversion of Types
	/////////////////////////////////////////////////////////////////
	template<ValueType T> struct ValueTypeToCTypeImpl;

	template<> struct ValueTypeToCTypeImpl<ValueType::Int8> { using Type = int8_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Int16> { using Type = int16_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Int32> { using Type = int32_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::Int64> { using Type = int64_t; };

	template<> struct ValueTypeToCTypeImpl<ValueType::UInt8> { using Type = uint8_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::UInt16> { using Type = uint16_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::UInt32> { using Type = uint32_t; };
	template<> struct ValueTypeToCTypeImpl<ValueType::UInt64> { using Type = uint64_t; };

	template<ValueType T>
	using ValueTypeToCType = ValueTypeToCTypeImpl<T>::Type;
	
	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::string ValueTypeToASM(ValueType type);

}