#include "dypch.h"
#include "Nodes.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

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
	Expression::Term::Int64Literal* Expression::Term::Int64Literal::New()
	{
		return s_Allocator.Construct<Int64Literal>();
	}

	Expression::Term::Int64Literal* Expression::Term::Int64Literal::New(const Token& token)
	{
		return s_Allocator.Construct<Int64Literal>(token);
	}

	Expression::Term::Identifier* Expression::Term::Identifier::New()
	{
		return s_Allocator.Construct<Identifier>();
	}

	Expression::Term::Identifier* Expression::Term::Identifier::New(const Token& token, VariableType variableType)
	{
		return s_Allocator.Construct<Identifier>(token, variableType);
	}

	Expression::Term* Expression::Term::New()
	{
		return s_Allocator.Construct<Term>();
	}

	Expression::Term* Expression::Term::New(Int64Literal* int64literal)
	{
		return s_Allocator.Construct<Term>(int64literal);
	}

	Expression::Term* Expression::Term::New(Identifier* identifier)
	{
		return s_Allocator.Construct<Term>(identifier);
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

	Expression* Expression::New(Term* term)
	{
		return s_Allocator.Construct<Expression>(term);
	}

	Expression* Expression::New(Binary* binary)
	{
		return s_Allocator.Construct<Expression>(binary);
	}



	// Constructors
	Expression::Term::Int64Literal::Int64Literal(const Token& token)
		: TokenObj(token)
	{
	}

	Expression::Term::Identifier::Identifier(const Token& token, VariableType variableType)
		: TokenObj(token), ResultType(variableType)
	{
	}

	Expression::Term::Term(Int64Literal* int64literal)
		: Int64LiteralObj(int64literal), ResultType(VariableType::Int64), TermType(Type::Int64Literal)
	{
	}

	Expression::Term::Term(Identifier* identifier)
		: IdentifierObj(identifier), ResultType(identifier->ResultType), TermType(Type::Int64Literal) // TODO: Support multiple times
	{
	}

	Expression::Binary::Addition::Addition(Expression* lhs, Expression* rhs)
		: LHS(lhs), RHS(rhs), ResultType(lhs->ResultType)
	{
	}

	Expression::Binary::Multiply::Multiply(Expression* lhs, Expression* rhs)
		: LHS(lhs), RHS(rhs), ResultType(lhs->ResultType)
	{
	}

	Expression::Binary::Binary(Addition* addition)
		: AdditionObj(addition), BinaryType(Expression::Binary::Type::Addition), ResultType(addition->LHS->ResultType)
	{
	}

	Expression::Binary::Binary(Multiply* multiply)
		: MultiplyObj(multiply), BinaryType(Expression::Binary::Type::Multiply), ResultType(multiply->LHS->ResultType)
	{
	}

	Expression::Expression(Term* term)
		: TermObj(term), ExpressionType(Type::Term), ResultType(term->ResultType)
	{
	}

	Expression::Expression(Binary* binary)
		: BinaryObj(binary), ExpressionType(Type::Binary), ResultType(binary->ResultType)
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
		case Expression::Type::Term:
		{
			switch (expr.TermObj->TermType)
			{
			case Expression::Term::Type::Int64Literal:	return Tokenizer::FormatToken(expr.TermObj->Int64LiteralObj->TokenObj);
			case Expression::Term::Type::Identifier:	return Tokenizer::FormatToken(expr.TermObj->IdentifierObj->TokenObj);

			default:
				DY_LOG_ERROR("Invalid Expression::Term::Type");
				break;
			}

			break;
		}
		case Expression::Type::Binary:			
		{
			switch (expr.BinaryObj->BinaryType)
			{
			// TODO: ...
			case Expression::Binary::Type::Addition:	
			case Expression::Binary::Type::Multiply:

			default:
				DY_LOG_ERROR("Invalid Expression::Binary::Type");
				break;
			}

			break;
		}

		default:
			break;
		}

		DY_LOG_ERROR("Invalid Expression::Type");
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

		DY_LOG_ERROR("Unnamed Statement::Type");
		return "Unnamed Statement::Type";
	}

	/////////////////////////////////////////////////////////////////
	// Helper variable functions
	/////////////////////////////////////////////////////////////////
	// Note: This function has to be manually updated
	VariableType GetVariableType(TokenType tokenType)
	{
		switch (tokenType)
		{
		case TokenType::Int64Literal:                       return VariableType::Int64;

		default:
			break;
		}

		DY_LOG_ERROR("TokenType::{0}, VariableType has not been defined.", Pulse::Enum::Name(tokenType));
		return VariableType::None;
	}

	// Note: This function has to be manually updated
	VariableType GetVariableType(Nodes::Expression::Term::Type termType)
	{
		switch (termType)
		{
		case Nodes::Expression::Term::Type::Int64Literal:   return VariableType::Int64;

		default:
			break;
		}

		DY_LOG_ERROR("Expression::Term::Type::{0}, VariableType has not been defined.", Pulse::Enum::Name(termType));
		return VariableType::None;
	}

	size_t VariableTypeSize(VariableType type)
	{
		switch (type)
		{
		case VariableType::Int64:     return sizeof(int64_t);

		default:
			break;
		}

		DY_LOG_ERROR("VariableType::{0}, size has not been defined.", Pulse::Enum::Name(type));
		return 0;
	}

	std::string VariableTypeToASM(VariableType type)
	{
		switch (type)
		{
		case VariableType::Int64:     return "QWORD";

		default:
			break;
		}

		DY_LOG_ERROR("VariableType::{0}, ASM type has not been defined.", Pulse::Enum::Name(type));
		return "Undefined ASM Type";
	}

}