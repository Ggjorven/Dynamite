#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <vector>
#include <variant>
#include <string>

namespace Dynamite::Language::Node
{

	// Base node.
	struct Base {};

	/////////////////////////////////////////////////////////////////
	// Forward declaration of all node types
	/////////////////////////////////////////////////////////////////
	struct Expression;
									
	struct TermExpr;
	struct BinaryExpr;
	struct AddressExpr;
	struct DereferenceExpr;

	// Note: Can be an expression or statement
	struct FunctionCall; 
									
	struct IdentifierTerm;
	struct LiteralTerm;
	struct ParenthesisTerm;

	struct BinaryAddition;
	struct BinarySubtraction;
	struct BinaryMultiplication;
	struct BinaryDivision;
	struct BinaryOR;
	struct BinaryAND;
	struct BinaryXOR;
									
	struct Statement;
									
	struct IfStatement;
	struct VariableStatement;
	struct AssignmentStatement;
	struct ScopeStatement;
	struct ReturnStatement;

	struct ElseIfBranch;
	struct ElseBranch;
	struct ConditionBranch;	

	struct FunctionDeclaration;
	struct FunctionDefinition;

	struct Function;

	struct Program;

	/////////////////////////////////////////////////////////////////
	// Types
	/////////////////////////////////////////////////////////////////
	enum class NodeType : uint8_t
	{
		Expression = 0,

		TermExpr,
		BinaryExpr,
		ReferenceExpr,
		AddressExpr,
		DereferenceExpr,

		FunctionCall,

		IdentifierTerm,
		LiteralTerm,
		ParenthesisTerm,

		BinaryAddition,
		BinarySubtraction,
		BinaryMultiplication,
		BinaryDivision,
		BinaryOR,
		BinaryAND,
		BinaryXOR,

		Statement,

		IfStatement,
		VariableStatement,
		AssignmentStatement,
		ScopeStatement,
		ReturnStatement,

		ElseIfBranch,
		ElseBranch,
		ConditionBranch,

		FunctionDeclaration,
		FunctionDefinition,

		Function,
	};

	/////////////////////////////////////////////////////////////////
	// Utilities
	/////////////////////////////////////////////////////////////////
	template<typename ...Types>
	using Variant = std::variant<Types...>;

	// Internal Ref type.
	template<typename T>
	using Ref = T*;

	constexpr const Ref<void> NullRef = nullptr;

	/////////////////////////////////////////////////////////////////
	// Memory
	/////////////////////////////////////////////////////////////////
	// Allocator for node types
	// Use Node::New<TNode> to allocate nodes.
	static Pulse::Memory::DynamicArenaAllocator Allocator((2ull * (1024 * 1024)));

	inline void Reset()
	{
		// Note: Currently not implemented
	}

	template<typename T, typename ...TArgs>
	inline Ref<T> New(TArgs&& ...args)
	{
		return Allocator.Construct<T>(std::forward<TArgs>(args)...);
	}

	// Only use after a New. Otherwise it will mess with another object.
	template<typename T>
	inline void Destroy(Ref<T> object) 
	{
		Allocator.Destroy<T>(object, true);
	}

	/////////////////////////////////////////////////////////////////
	// Debug
	/////////////////////////////////////////////////////////////////
	constexpr const char* TabString = "    ";

}