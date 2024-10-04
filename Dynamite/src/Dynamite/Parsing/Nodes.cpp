#include "dypch.h"
#include "Nodes.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#include <Pulse/Types/TypeUtils.hpp>
#include <Pulse/Memory/ArenaAllocator.hpp>

namespace Dynamite::Nodes
{

	constexpr static const size_t s_MaxSize = 2ull * (1024 * 1024);
	static Pulse::Memory::ArenaAllocator s_Allocator(s_MaxSize);

	
	
	/////////////////////////////////////////////////////////////////
	// Expression nodes
	/////////////////////////////////////////////////////////////////
	// New "operators"
	Expression::Int64Literal* Expression::Int64Literal::New()
	{
		return s_Allocator.Construct<Int64Literal>();
	}

	Expression::Int64Literal* Expression::Int64Literal::New(const Token& token)
	{
		return s_Allocator.Construct<Int64Literal>(token);
	}

	Expression::Identifier* Expression::Identifier::New()
	{
		return s_Allocator.Construct<Identifier>();
	}

	Expression::Identifier* Expression::Identifier::New(const Token& token)
	{
		return s_Allocator.Construct<Identifier>(token);
	}

	Expression::Binary::Addition* Expression::Binary::Addition::New()
	{
		return s_Allocator.Construct<Addition>();
	}

	Expression::Binary::Addition* Expression::Binary::Addition::New(Expression* lhs, Expression* rhs)
	{
		return s_Allocator.Construct<Addition>(lhs, rhs);
	}

	Expression::Binary::Multiply* Expression::Binary::Multiply::New()
	{
		return s_Allocator.Construct<Multiply>();
	}

	Expression::Binary::Multiply* Expression::Binary::Multiply::New(Expression* lhs, Expression* rhs)
	{
		return s_Allocator.Construct<Multiply>(lhs, rhs);
	}

	Expression::Binary* Expression::Binary::New()
	{
		return s_Allocator.Construct<Binary>();
	}

	Expression::Binary* Expression::Binary::New(Addition* addition)
	{
		return s_Allocator.Construct<Binary>(addition);
	}

	Expression::Binary* Expression::Binary::New(Multiply* multiply)
	{
		return s_Allocator.Construct<Binary>(multiply);
	}

	Expression* Expression::New()
	{
		return s_Allocator.Construct<Expression>();
	}

	Expression* Expression::New(Int64Literal* int64literal)
	{
		return s_Allocator.Construct<Expression>(int64literal);
	}

	Expression* Expression::New(Identifier* identifier)
	{
		return s_Allocator.Construct<Expression>(identifier);
	}

	Expression* Expression::New(Binary* binary)
	{
		return s_Allocator.Construct<Expression>(binary);
	}



	// Constructors
	Expression::Int64Literal::Int64Literal(const Token& token)
		: TokenObj(token)
	{
	}

	Expression::Identifier::Identifier(const Token& token)
		: TokenObj(token)
	{
	}

	Expression::Binary::Addition::Addition(Expression* lhs, Expression* rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	Expression::Binary::Multiply::Multiply(Expression* lhs, Expression* rhs)
		: LHS(lhs), RHS(rhs)
	{
	}

	Expression::Binary::Binary(Addition* addition)
		: AdditionObj(addition), BinaryType(Expression::Binary::Type::Addition)
	{
	}

	Expression::Binary::Binary(Multiply* multiply)
		: MultiplyObj(multiply), BinaryType(Expression::Binary::Type::Multiply)
	{
	}

	Expression::Expression(Int64Literal* int64literal)
		: Int64LiteralObj(int64literal), ExpressionType(Type::Int64Literal)
	{
	}

	Expression::Expression(Identifier* identifier)
		: IdentifierObj(identifier), ExpressionType(Type::Identifier)
	{
	}

	Expression::Expression(Binary* binary)
		: BinaryObj(binary), ExpressionType(Type::Binary)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Statement nodes
	/////////////////////////////////////////////////////////////////
	// New "operators"
	Statement::Let* Statement::Let::New()
	{
		return s_Allocator.Construct<Let>();
	}

	Statement::Let* Statement::Let::New(const Token& token, Expression* expression)
	{
		return s_Allocator.Construct<Let>(token, expression);
	}

	Statement::Exit* Statement::Exit::New()
	{
		return s_Allocator.Construct<Exit>();
	}

	Statement::Exit* Statement::Exit::New(Expression* expression)
	{
		return s_Allocator.Construct<Exit>(expression);
	}

	Statement* Statement::New()
	{
		return s_Allocator.Construct<Statement>();
	}

	Statement* Statement::New(Let* let)
	{
		return s_Allocator.Construct<Statement>(let);
	}

	Statement* Statement::New(Exit* exit)
	{
		return s_Allocator.Construct<Statement>(exit);
	}



	// Constructors
	Statement::Let::Let(const Token& token, Expression* expression)
		: TokenObj(token), ExpressionObj(expression)
	{
	}

	Statement::Exit::Exit(Expression* expression)
		: ExpressionObj(expression)
	{
	}

	Statement::Statement(Let* let)
		: LetObj(let), StatementType(Type::Let)
	{
	}

	Statement::Statement(Exit* exit)
		: ExitObj(exit), StatementType(Type::Exit)
	{
	}

	/////////////////////////////////////////////////////////////////
	// Debug print function
	/////////////////////////////////////////////////////////////////
	// Note: This function has to be manually updated
	std::string FormatExpressionData(const Expression& expr)
	{
		switch (expr.ExpressionType)
		{
		case Expression::Type::Int64Literal:
			return Tokenizer::FormatToken(expr.Int64LiteralObj->TokenObj);
		case Expression::Type::Identifier:		
			return Tokenizer::FormatToken(expr.IdentifierObj->TokenObj);
		case Expression::Type::Binary:			
			return "TODO: ...";

		default:
			break;
		}

		return "Invalid Expression::Type";
	}

	// Note: This function has to be manually updated
	std::string FormatStatementData(const Statement& statement)
	{
		switch (statement.StatementType)
		{
		case Statement::Type::Let:
			return Pulse::Text::Format("{0}([{1}])", Tokenizer::FormatToken(statement.LetObj->TokenObj), FormatExpressionData(*(statement.LetObj->ExpressionObj)));
		case Statement::Type::Exit:
			return Pulse::Text::Format("{0}", FormatExpressionData(*(statement.ExitObj->ExpressionObj)));

		default:
			break;
		}

		return "Unnamed Statement::Type";
	}

}