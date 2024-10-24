#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Tokens/Token.hpp"

#include <cstdint>
#include <vector>
#include <variant>
#include <string>

namespace Dynamite::Node
{

	/////////////////////////////////////////////////////////////////
	// Forward declaration of all node types
	/////////////////////////////////////////////////////////////////
	struct Expression;				
									
	struct TermExpr;				
	struct BinaryExpr;				
									
	struct IdentifierTerm;			
	struct LiteralTerm;				
	struct ParenthesisTerm;		

	struct BinaryAddition;
	struct BinarySubtraction;
	struct BinaryMultiplication;
	struct BinaryDivision;
									
	struct Statement;				
									
	struct IfStatement;
	struct VariableStatement;
	struct AssignmentStatement;

	struct ElseIfBranch;			
	struct ElseBranch;				
	struct ScopeStatement;			
	struct ConditionBranch;			

	struct Function;
	struct Program;

	/////////////////////////////////////////////////////////////////
	// Utilities
	/////////////////////////////////////////////////////////////////
	template<typename ...Types>
	using Variant = std::variant<Types...>;

	// Internal reference type.
	template<typename T>
	using Reference = T*;

	constexpr const Reference<void> NullRef = nullptr;

	// This can change based on the Reference<T> type.
	// Currently it's a pointer, so the _DEREF 'operator' should be nothing
	#define NODE_DEREF 

	/////////////////////////////////////////////////////////////////
	// Memory
	/////////////////////////////////////////////////////////////////

	// Allocator for node types
	// Use DynamicArenaAllocator::Construct<T> to allocate nodes.
	static Pulse::Memory::DynamicArenaAllocator Allocator((2ull * (1024 * 1024)));

	template<typename T, typename ...TArgs>
	inline Reference<T> New(TArgs&& ...args)
	{
		return Allocator.Construct<T>(std::forward<TArgs>(args)...);
	}

	// Only use after a New. Otherwise it will mess with another object.
	template<typename T>
	inline void Destroy(Reference<T> object) 
	{
		Allocator.Destroy<T>(object, true);
	}

	/////////////////////////////////////////////////////////////////
	// Logging
	/////////////////////////////////////////////////////////////////
	constexpr const char* TabString = "    ";

}