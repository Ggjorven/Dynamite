#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Types/Type.hpp"

#include "Dynamite/Parser/Nodes/All.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include <Pulse/Types/TypeUtils.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Tracker
	/////////////////////////////////////////////////////////////////
	// Note: This class is to keep track of what's currently being parsed.
	class Tracker 
	{
	public:
		template<typename TNode, typename ...TArgs>
		Node::Reference<TNode> New(TArgs&& ...args);

		template<typename TNode>
		Node::Reference<TNode> Return();

	public:
		template<typename TNode>
		Node::Reference<TNode> Pop();

		template<typename TNode>
		Node::Reference<TNode> Get();

	private:
		std::vector<Node::Reference<Node::Expression>> m_Expressions = {};
		
		std::vector<Node::Reference<Node::TermExpr>> m_TermExprs = {};
		std::vector<Node::Reference<Node::BinaryExpr>> m_BinaryExprs = {};
		
		std::vector<Node::Reference<Node::FunctionCall>> m_FunctionCalls = {};
		
		std::vector<Node::Reference<Node::IdentifierTerm>> m_IdentifierTerms = {};
		std::vector<Node::Reference<Node::LiteralTerm>> m_LiteralTerms = {};
		std::vector<Node::Reference<Node::ParenthesisTerm>> m_ParenthesisTerms = {};
		
		std::vector<Node::Reference<Node::BinaryAddition>> m_BinaryAdditions = {};
		std::vector<Node::Reference<Node::BinarySubtraction>> m_BinarySubtractions = {};
		std::vector<Node::Reference<Node::BinaryMultiplication>> m_BinaryMultiplications = {};
		std::vector<Node::Reference<Node::BinaryDivision>> m_BinaryDivisions = {};
		
		std::vector<Node::Reference<Node::Statement>> m_Statements = {};
		
		std::vector<Node::Reference<Node::IfStatement>> m_IfStatements = {};
		std::vector<Node::Reference<Node::VariableStatement>> m_VariableStatements = {};
		std::vector<Node::Reference<Node::AssignmentStatement>> m_AssignmentStatements = {};
		std::vector<Node::Reference<Node::ScopeStatement>> m_ScopeStatements = {};
		
		std::vector<Node::Reference<Node::ElseIfBranch>> m_ElseIfBranchs = {};
		std::vector<Node::Reference<Node::ElseBranch>> m_ElseBranchs = {};
		std::vector<Node::Reference<Node::ConditionBranch>> m_ConditionBranchs = {};

		std::vector<Node::Reference<Node::Function>> m_Functions = {};
	};

	/////////////////////////////////////////////////////////////////
	// Macros
	/////////////////////////////////////////////////////////////////
	#define TRACKER_TRACK(node)																		\
		if constexpr (Pulse::Types::Same<TNode, Node::node>)										\
		{																							\
			return m_##node##s.emplace_back(Node::New<Node::node>(std::forward<TArgs>(args)...));	\
		}

	#define TRACKER_POP(node)																		\
		if constexpr (Pulse::Types::Same<TNode, Node::node>)										\
		{																							\
			if (!m_##node##s.empty())																\
			{																						\
				Node::Reference<Node::node> ret = m_##node##s.back();								\
				m_##node##s.pop_back();																\
				return ret;																			\
			}																						\
			return (Node::Reference<TNode>)Node::NullRef;											\
		}

	#define TRACKER_GET(node)																		\
		if constexpr (Pulse::Types::Same<TNode, Node::node>)										\
		{																							\
			if (!m_##node##s.empty())																\
			{																						\
				return m_##node##s.back();															\
			}																						\
			return (Node::Reference<TNode>)Node::NullRef;											\
		}

	/////////////////////////////////////////////////////////////////
	// Templated functions
	/////////////////////////////////////////////////////////////////
	template<typename TNode, typename ...TArgs>
	Node::Reference<TNode> Tracker::New(TArgs&& ...args)
	{
		TRACKER_TRACK(Expression);

		TRACKER_TRACK(TermExpr);
		TRACKER_TRACK(BinaryExpr);

		TRACKER_TRACK(FunctionCall);

		TRACKER_TRACK(IdentifierTerm);
		TRACKER_TRACK(LiteralTerm);
		TRACKER_TRACK(ParenthesisTerm);

		TRACKER_TRACK(BinaryAddition);
		TRACKER_TRACK(BinarySubtraction);
		TRACKER_TRACK(BinaryMultiplication);
		TRACKER_TRACK(BinaryDivision);

		TRACKER_TRACK(Statement);

		TRACKER_TRACK(IfStatement);
		TRACKER_TRACK(VariableStatement);
		TRACKER_TRACK(AssignmentStatement);
		TRACKER_TRACK(ScopeStatement);

		TRACKER_TRACK(ElseIfBranch);
		TRACKER_TRACK(ElseBranch);
		TRACKER_TRACK(ConditionBranch);

		TRACKER_TRACK(Function);

		//static_assert(false, "Unhandled TNode passed in.")
		return (Node::Reference<TNode>)Node::NullRef;
	}

	template<typename TNode>
	Node::Reference<TNode> Tracker::Return()
	{
		return Pop<TNode>();
	}

	template<typename TNode>
	inline Node::Reference<TNode> Tracker::Pop()
	{
		TRACKER_POP(Expression);

		TRACKER_POP(TermExpr);
		TRACKER_POP(BinaryExpr);

		TRACKER_POP(FunctionCall);

		TRACKER_POP(IdentifierTerm);
		TRACKER_POP(LiteralTerm);
		TRACKER_POP(ParenthesisTerm);

		TRACKER_POP(BinaryAddition);
		TRACKER_POP(BinarySubtraction);
		TRACKER_POP(BinaryMultiplication);
		TRACKER_POP(BinaryDivision);

		TRACKER_POP(Statement);

		TRACKER_POP(IfStatement);
		TRACKER_POP(VariableStatement);
		TRACKER_POP(AssignmentStatement);
		TRACKER_POP(ScopeStatement);

		TRACKER_POP(ElseIfBranch);
		TRACKER_POP(ElseBranch);
		TRACKER_POP(ConditionBranch);

		TRACKER_POP(Function);

		//static_assert(false, "Unhandled TNode passed in.")
		return (Node::Reference<TNode>)Node::NullRef;
	}

	template<typename TNode>
	Node::Reference<TNode> Tracker::Get()
	{
		TRACKER_GET(Expression);

		TRACKER_GET(TermExpr);
		TRACKER_GET(BinaryExpr);

		TRACKER_GET(FunctionCall);

		TRACKER_GET(IdentifierTerm);
		TRACKER_GET(LiteralTerm);
		TRACKER_GET(ParenthesisTerm);

		TRACKER_GET(BinaryAddition);
		TRACKER_GET(BinarySubtraction);
		TRACKER_GET(BinaryMultiplication);
		TRACKER_GET(BinaryDivision);

		TRACKER_GET(Statement);

		TRACKER_GET(IfStatement);
		TRACKER_GET(VariableStatement);
		TRACKER_GET(AssignmentStatement);
		TRACKER_GET(ScopeStatement);

		TRACKER_GET(ElseIfBranch);
		TRACKER_GET(ElseBranch);
		TRACKER_GET(ConditionBranch);

		TRACKER_GET(Function);

		//static_assert(false, "Unhandled TNode passed in.")
		return (Node::Reference<TNode>)Node::NullRef;
	}

}