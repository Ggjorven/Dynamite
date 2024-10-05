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
	// Currently it's a pointer, so the _DEREFERENCE operator should be nothing
	#define _DEREF 

	namespace
	{
		constexpr static const size_t s_MaxSize = 2ull * (1024 * 1024); // 2 MB
		static Pulse::Memory::DynamicArenaAllocator s_Allocator(s_MaxSize);
	}

	/////////////////////////////////////////////////////////////////
	// Constructors
	/////////////////////////////////////////////////////////////////
	IntegerLiteralTerm::IntegerLiteralTerm(const Token& token) : TokenObj(token) {}
	
	IdentifierTerm::IdentifierTerm(const Token& token) : TokenObj(token) {}

	TermExpr::TermExpr(Reference<IntegerLiteralTerm> integerLiteral) : TermObj(integerLiteral) {}
	TermExpr::TermExpr(Reference<IdentifierTerm> identifier) : TermObj(identifier) {}

	BinaryExpr::BinaryExpr(Type binaryType, Reference<Expression> lhs, Reference<Expression> rhs) : BinaryType(binaryType), LHS(lhs), RHS(rhs) {}

	Expression::Expression(Reference<TermExpr> term, ValueType type) : ExprObj(term), Type(type) {}
	Expression::Expression(Reference<BinaryExpr> binary, ValueType type) : ExprObj(binary), Type(type) {}



	VariableStatement::VariableStatement(const Token& token, Reference<Expression> expr) : TokenObj(token), ExprObj(expr) {}

	ExitStatement::ExitStatement(Reference<Expression> expr) : ExprObj(expr) {}

	Statement::Statement(Reference<VariableStatement> var) : StatementObj(var) {}
	Statement::Statement(Reference<ExitStatement> exit) : StatementObj(exit) {}

	/////////////////////////////////////////////////////////////////
	// Custom allocator functions
	/////////////////////////////////////////////////////////////////
	Reference<IntegerLiteralTerm> IntegerLiteralTerm::New(const Token& token) { return _DEREF s_Allocator.Construct<IntegerLiteralTerm>(token); }

	Reference<IdentifierTerm> IdentifierTerm::New(const Token& token) { return _DEREF s_Allocator.Construct<IdentifierTerm>(token); }

	Reference<TermExpr> TermExpr::New(Reference<IntegerLiteralTerm> integerLiteral) { return _DEREF s_Allocator.Construct<TermExpr>(integerLiteral); }
	Reference<TermExpr> TermExpr::New(Reference<IdentifierTerm> identifier) { return _DEREF s_Allocator.Construct<TermExpr>(identifier); }

	Reference<BinaryExpr> BinaryExpr::New(Type binaryType, Reference<Expression> lhs, Reference<Expression> rhs) { return _DEREF s_Allocator.Construct<BinaryExpr>(binaryType, lhs, rhs); }

	Reference<Expression> Expression::New(Reference<TermExpr> term, ValueType type) { return _DEREF s_Allocator.Construct<Expression>(term, type); }
	Reference<Expression> Expression::New(Reference<BinaryExpr> binary, ValueType type) { return _DEREF s_Allocator.Construct<Expression>(binary, type); }



	Reference<VariableStatement> VariableStatement::New(const Token& token, Reference<Expression> expr) { return _DEREF s_Allocator.Construct<VariableStatement>(token, expr); }

	Reference<ExitStatement> ExitStatement::New(Reference<Expression> expr) { return _DEREF s_Allocator.Construct<ExitStatement>(expr); }

	Reference<Statement> Statement::New(Reference<VariableStatement> var) { return _DEREF s_Allocator.Construct<Statement>(var); }
	Reference<Statement> Statement::New(Reference<ExitStatement> exit) { return _DEREF s_Allocator.Construct<Statement>(exit); }

	/////////////////////////////////////////////////////////////////
	// Member functions
	/////////////////////////////////////////////////////////////////
	Token TermExpr::GetToken()
	{
		return std::visit([](auto&& obj) -> Token
		{
			if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<IntegerLiteralTerm>>)
				return obj->TokenObj;
			else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<IdentifierTerm>>)
				return obj->TokenObj;

			return {};
		}, TermObj);
	}

	/////////////////////////////////////////////////////////////////
	// Helper functions
	/////////////////////////////////////////////////////////////////
	// Note: Has to be manually updated
	std::string FormatExpressionData(const Reference<Expression> expr)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<TermExpr>>)
			{
				return std::visit([](auto&& obj) -> std::string
				{
					if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<IntegerLiteralTerm>>)
						return Tokenizer::FormatToken(obj->TokenObj);
					else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<IdentifierTerm>>)
						return Tokenizer::FormatToken(obj->TokenObj);

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
				return Pulse::Text::Format("{0}([{1}])", Tokenizer::FormatToken(obj->TokenObj), FormatExpressionData(obj->ExprObj));
			}
			else if constexpr (Pulse::Types::Same<Pulse::Types::Clean<decltype(obj)>, Reference<ExitStatement>>)
			{
				return Pulse::Text::Format("{0}", FormatExpressionData(obj->ExprObj));
			}

			return "Undefined Statement Data";
		},
		statement->StatementObj);
	}

}