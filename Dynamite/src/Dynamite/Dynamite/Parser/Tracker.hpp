#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/Nodes/Program.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include <Pulse/Types/TypeUtils.hpp>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Tracker
	/////////////////////////////////////////////////////////////////
	class Tracker // Note: This class is to keep track of what's currently being parsed. // TODO: Cleanup
	{
	public:
		template<typename TNode, typename ...TArgs>
		Language::Node::Ref<TNode> New(TArgs&& ...args);

		template<typename TNode>
		Language::Node::Ref<TNode> Return();

	public:
		template<typename TNode>
		Language::Node::Ref<TNode> Pop();

		template<typename TNode>
		Language::Node::Ref<TNode> Get();

	private:
		std::vector<Language::Node::Ref<Language::Node::Expression>> m_Expressions = {};

		std::vector<Language::Node::Ref<Language::Node::TermExpr>> m_TermExprs = {};
		std::vector<Language::Node::Ref<Language::Node::BinaryExpr>> m_BinaryExprs = {};
		std::vector<Language::Node::Ref<Language::Node::AddressExpr>> m_ReferenceExprs = {};
		std::vector<Language::Node::Ref<Language::Node::AddressExpr>> m_AddressExprs = {};
		std::vector<Language::Node::Ref<Language::Node::DereferenceExpr>> m_DereferenceExprs = {};

		std::vector<Language::Node::Ref<Language::Node::FunctionCall>> m_FunctionCalls = {};

		std::vector<Language::Node::Ref<Language::Node::IdentifierTerm>> m_IdentifierTerms = {};
		std::vector<Language::Node::Ref<Language::Node::LiteralTerm>> m_LiteralTerms = {};
		std::vector<Language::Node::Ref<Language::Node::ParenthesisTerm>> m_ParenthesisTerms = {};

		std::vector<Language::Node::Ref<Language::Node::BinaryAddition>> m_BinaryAdditions = {};
		std::vector<Language::Node::Ref<Language::Node::BinarySubtraction>> m_BinarySubtractions = {};
		std::vector<Language::Node::Ref<Language::Node::BinaryMultiplication>> m_BinaryMultiplications = {};
		std::vector<Language::Node::Ref<Language::Node::BinaryDivision>> m_BinaryDivisions = {};

		std::vector<Language::Node::Ref<Language::Node::Statement>> m_Statements = {};

		std::vector<Language::Node::Ref<Language::Node::IfStatement>> m_IfStatements = {};
		std::vector<Language::Node::Ref<Language::Node::VariableStatement>> m_VariableStatements = {};
		std::vector<Language::Node::Ref<Language::Node::AssignmentStatement>> m_AssignmentStatements = {};
		std::vector<Language::Node::Ref<Language::Node::ScopeStatement>> m_ScopeStatements = {};

		std::vector<Language::Node::Ref<Language::Node::ElseIfBranch>> m_ElseIfBranchs = {};
		std::vector<Language::Node::Ref<Language::Node::ElseBranch>> m_ElseBranchs = {};
		std::vector<Language::Node::Ref<Language::Node::ConditionBranch>> m_ConditionBranchs = {};

		// Note: "Function" does not get tracked.
		std::vector<Language::Node::Ref<Language::Node::FunctionDefinition>> m_FunctionDefinitions = {};
		std::vector<Language::Node::Ref<Language::Node::FunctionDeclaration>> m_FunctionDeclarations = {};
	};

	/////////////////////////////////////////////////////////////////
	// Macros
	/////////////////////////////////////////////////////////////////
	#define TRACKER_TRACK(node)																							\
		if constexpr (Pulse::Types::Same<TNode, Language::Node::node>)													\
		{																												\
			return m_##node##s.emplace_back(Language::Node::New<Language::Node::node>(std::forward<TArgs>(args)...));	\
		}

	#define TRACKER_POP(node)																							\
		if constexpr (Pulse::Types::Same<TNode, Language::Node::node>)													\
		{																												\
			if (!m_##node##s.empty())																					\
			{																											\
				Language::Node::Ref<Language::Node::node> ret = m_##node##s.back();										\
				m_##node##s.pop_back();																					\
				return ret;																								\
			}																											\
			return (Language::Node::Ref<TNode>)Language::Node::NullRef;													\
		}

	#define TRACKER_GET(node)																							\
		if constexpr (Pulse::Types::Same<TNode, Language::Node::node>)													\
		{																												\
			if (!m_##node##s.empty())																					\
			{																											\
				return m_##node##s.back();																				\
			}																											\
			return (Language::Node::Ref<TNode>)Language::Node::NullRef;													\
		}

	/////////////////////////////////////////////////////////////////
	// Templated functions
	/////////////////////////////////////////////////////////////////
	template<typename TNode, typename ...TArgs>
	Language::Node::Ref<TNode> Tracker::New(TArgs&& ...args)
	{
		TRACKER_TRACK(Expression);

		TRACKER_TRACK(TermExpr);
		TRACKER_TRACK(BinaryExpr);
		TRACKER_TRACK(ReferenceExpr);
		TRACKER_TRACK(AddressExpr);
		TRACKER_TRACK(DereferenceExpr);

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

		TRACKER_TRACK(FunctionDeclaration);
		TRACKER_TRACK(FunctionDefinition);

		return (Language::Node::Ref<TNode>)Language::Node::NullRef;
	}

	template<typename TNode>
	Language::Node::Ref<TNode> Tracker::Return()
	{
		return Pop<TNode>();
	}

	template<typename TNode>
	Language::Node::Ref<TNode> Tracker::Pop()
	{
		TRACKER_POP(Expression);

		TRACKER_POP(TermExpr);
		TRACKER_POP(BinaryExpr);
		TRACKER_POP(ReferenceExpr);
		TRACKER_POP(AddressExpr);
		TRACKER_POP(DereferenceExpr);

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

		TRACKER_POP(FunctionDeclaration);
		TRACKER_POP(FunctionDefinition);

		return (Language::Node::Ref<TNode>)Language::Node::NullRef;
	}

	template<typename TNode>
	Language::Node::Ref<TNode> Tracker::Get()
	{
		TRACKER_GET(Expression);

		TRACKER_GET(TermExpr);
		TRACKER_GET(BinaryExpr);
		TRACKER_GET(ReferenceExpr);
		TRACKER_GET(AddressExpr);
		TRACKER_GET(DereferenceExpr);

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

		TRACKER_GET(FunctionDeclaration);
		TRACKER_GET(FunctionDefinition);

		return (Language::Node::Ref<TNode>)Language::Node::NullRef;
	}

}