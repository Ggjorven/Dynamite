#include "dypch.h"
#include "Nodes.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

#include <Pulse/Types/TypeUtils.hpp>

namespace Dynamite::Node
{

	// This can change based on the Reference<T> type.
	// Currently it's a pointer, so the _DEREF 'operator' should be nothing
	#define _DEREF 

	namespace
	{
		constexpr static const size_t s_MaxSize = 2ull * (1024 * 1024); // 2 MB
		static Pulse::Memory::DynamicArenaAllocator s_Allocator(s_MaxSize);
	}

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	LiteralTerm::LiteralTerm(Type literalType, const Token& token) : LiteralType(literalType), TokenObj(token) {}
	
	IdentifierTerm::IdentifierTerm(const Token& token) : TokenObj(token) {}

	ParenthesisTerm::ParenthesisTerm(Reference<Expression> expr) : ExprObj(expr) {}

	TermExpr::TermExpr(Reference<LiteralTerm> literalTerm) : TermObj(literalTerm) {}
	TermExpr::TermExpr(Reference<IdentifierTerm> identifier) : TermObj(identifier) {}
	TermExpr::TermExpr(Reference<ParenthesisTerm> parenthesis) : TermObj(parenthesis) {}

	BinaryExpr::BinaryExpr(Type binaryType, Reference<Expression> lhs, Reference<Expression> rhs) : BinaryType(binaryType), LHS(lhs), RHS(rhs) {}

	Expression::Expression(ValueType type, Reference<TermExpr> term) : Type(type), ExprObj(term) {}
	Expression::Expression(ValueType type, Reference<BinaryExpr> binary) : Type(type), ExprObj(binary) {}



	VariableStatement::VariableStatement(ValueType type, const Token& token, Reference<Expression> expr) : Type(type), TokenObj(token), ExprObj(expr) {}

	ExitStatement::ExitStatement(Reference<Expression> expr) : ExprObj(expr) {}

	Statement::Statement(Reference<VariableStatement> var) : StatementObj(var) {}
	Statement::Statement(Reference<ExitStatement> exit) : StatementObj(exit) {}

	/////////////////////////////////////////////////////////////////
	// Custom allocator functions
	/////////////////////////////////////////////////////////////////
	Reference<LiteralTerm> LiteralTerm::New(Type literalType, const Token& token) { return _DEREF s_Allocator.Construct<LiteralTerm>(literalType, token); }

	Reference<IdentifierTerm> IdentifierTerm::New(const Token& token) { return _DEREF s_Allocator.Construct<IdentifierTerm>(token); }

	Reference<ParenthesisTerm> ParenthesisTerm::New(Reference<Expression> expr) { return _DEREF s_Allocator.Construct<ParenthesisTerm>(expr); }

	Reference<TermExpr> TermExpr::New(Reference<LiteralTerm> literalTerm) { return _DEREF s_Allocator.Construct<TermExpr>(literalTerm); }
	Reference<TermExpr> TermExpr::New(Reference<IdentifierTerm> identifier) { return _DEREF s_Allocator.Construct<TermExpr>(identifier); }
	Reference<TermExpr> TermExpr::New(Reference<ParenthesisTerm> parenthesis) { return _DEREF s_Allocator.Construct<TermExpr>(parenthesis); }

	Reference<BinaryExpr> BinaryExpr::New(Type binaryType, Reference<Expression> lhs, Reference<Expression> rhs) { return _DEREF s_Allocator.Construct<BinaryExpr>(binaryType, lhs, rhs); }

	Reference<Expression> Expression::New(ValueType type, Reference<TermExpr> term) { return _DEREF s_Allocator.Construct<Expression>(type, term); }
	Reference<Expression> Expression::New(ValueType type, Reference<BinaryExpr> binary) { return _DEREF s_Allocator.Construct<Expression>(type, binary); }



	Reference<VariableStatement> VariableStatement::New(ValueType type, const Token& token, Reference<Expression> expr) { return _DEREF s_Allocator.Construct<VariableStatement>(type, token, expr); }

	Reference<ExitStatement> ExitStatement::New(Reference<Expression> expr) { return _DEREF s_Allocator.Construct<ExitStatement>(expr); }

	Reference<Statement> Statement::New(Reference<VariableStatement> var) { return _DEREF s_Allocator.Construct<Statement>(var); }
	Reference<Statement> Statement::New(Reference<ExitStatement> exit) { return _DEREF s_Allocator.Construct<Statement>(exit); }

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	std::optional<size_t> GetBinaryExprPrecendce(BinaryExpr::Type type)
	{
		switch (type)
		{
		case BinaryExpr::Type::Addition:
		case BinaryExpr::Type::Subtraction:
			return 0;

		case BinaryExpr::Type::Multiplication:
		case BinaryExpr::Type::Division:
			return 1;

		// TODO: Additional binary operators

		default:
			break;
		}

		//DY_LOG_ERROR("BinaryExpr::Type's precendce level has not been implemented.");
		return {};
	}

	// Note: Has to be manually updated
	std::string FormatExpressionData(const Reference<Expression> expr)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<TermExpr>>)
			{
				return std::visit([](auto&& obj) -> std::string
				{
					if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<LiteralTerm>>)
						return FormatToken(obj->TokenObj);
					else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<IdentifierTerm>>)
						return FormatToken(obj->TokenObj);
					else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<ParenthesisTerm>>)
							return Pulse::Text::Format("Parenthesis: ({0})", FormatExpressionData(obj->ExprObj));

					return "Undefined Term Type";
				}, obj->TermObj);
			}
			else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<BinaryExpr>>)
			{
				return Pulse::Text::Format("(LHS: {0}, '{1}' RHS: {2})", FormatExpressionData(obj->LHS), (char)obj->BinaryType, FormatExpressionData(obj->RHS));
			}

			return "Undefined Expression Data";
		},
		expr->ExprObj);
	}

	// Note: Has to be manually updated
	std::string FormatStatementData(const Reference<Statement> statement)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<VariableStatement>>)
			{
				return Pulse::Text::Format("[Variable({0})] - {1}([{2}])", ValueTypeToStr(obj->Type), FormatToken(obj->TokenObj), FormatExpressionData(obj->ExprObj));
			}
			else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<ExitStatement>>)
			{
				return Pulse::Text::Format("[Exit] - {0}", FormatExpressionData(obj->ExprObj));
			}

			return "Undefined Statement Data";
		},
		statement->StatementObj);
	}

}