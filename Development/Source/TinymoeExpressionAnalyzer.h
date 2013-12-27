#ifndef VCZH_TINYMOEEXPRESSIONANALYZER
#define VCZH_TINYMOEEXPRESSIONANALYZER

#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	Symbol
	*************************************************************/

	enum class GrammarType
	{
		Name,					// for identifier list,				e.g. [repeat with] the current number [from] 1 [to] [sum from] 1 [to] 10
		PrimitiveExpression,	// for primitive expression,		e.g. repeat with the current number from 1 to sum from 1 to [10]
		Expression,				// for all kinds of expressions,	e.g. repeat with the current number from [1] to [sum from [1] to 10]
		Argument,				// for creating a new symbol,		e.g. repeat with [the current number] from 1 to sum from 1 to 10
	};

	class GrammarFragment
	{
	public:
		typedef shared_ptr<GrammarFragment>			Ptr;
		typedef vector<Ptr>							List;

		GrammarType					type;
		CodeToken::List				identifiers;
	};

	class GrammarSymbol
	{
	public:
		typedef shared_ptr<GrammarFragment>			Ptr;
		typedef vector<Ptr>							List;
		typedef multimap<string, Ptr>				MultiMap;

		GrammarFragment::List		grammar;		// grammar for this symbol
		bool						statement;		// true means this symbol is a statement
													// a statement cannot be an expression
													// the top invoke expression's function of a statement should reference to a statement symbol
		string						uniqueId;		// a string that identifies the grammar structure
	};

	/*************************************************************
	Symbol Stack
	*************************************************************/

	class GrammarStackItem
	{
	public:
		typedef shared_ptr<GrammarFragment>			Ptr;
		typedef vector<Ptr>							List;

		GrammarSymbol::List			expressionSymbols;		// all symbols for a legal expression
		GrammarSymbol::List			statementSymbols;		// all symbols for a legal statement
	};

	class GrammarStack
	{
	public:
		typedef shared_ptr<GrammarFragment>			Ptr;

		GrammarStackItem::List		stackItems;				// available symbols organized like a scope based structure
		GrammarSymbol::MultiMap		availableSymbols;		// available symbols grouped by the unique identifier
															// the last symbol overrides all other symbols in the same group
	};

	/*************************************************************
	Expression
	*************************************************************/

	class Expression : public CodeFragment
	{
	public:
		typedef shared_ptr<Expression>				Ptr;
		typedef vector<Ptr>							List;
	};

	class LiteralExpression : public Expression
	{
	public:
		CodeToken					token;
	};

	class ReferenceExpression : public Expression
	{
	public:
		GrammarSymbol::Ptr			symbol;
	};

	class InvokeExpression : public Expression
	{
	public:
		Expression::Ptr				function;
		Expression::List			arguments;
	};
}

#endif