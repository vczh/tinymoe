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
		Name,					// for identifier list,				e.g. [repeat with] the current number [from] 1 [to] 100
		PrimitiveExpression,	// for primitive expression,		e.g. sum from 1 to [10]
		List,					// for tuple (marshalled as array),	e.g. set names to collection of [("a", "b", "c")]
		Assignable,				// for left value expression,		e.g. [field unique identifier of person]
								//									e.g. [a variable]
		Argument,				// for creating a new symbol,		e.g. repeat with [the current number] from 1 to sum from 1 to 10
		Expression,				// for all kinds of expressions,	e.g. repeat with the current number from [1] to [100]
		Type,					// for type name,					e.g. set names to new [hash set]
	};

	class GrammarFragment
	{
	public:
		typedef shared_ptr<GrammarFragment>			Ptr;
		typedef vector<Ptr>							List;

		GrammarType					type = GrammarType::Name;
		vector<string>				identifiers;
	};

	enum class PredefinedSymbolType
	{
		Custom,					// user defined symbol
		End,					// (statement)	end
		Select,					// (statement)	select <expression>
		Case,					// (statement)	case <expression>
		TailCall,				// (statement)	tail call <expression>
		Assign,					// (statement)	set <assignable> to <expression>
		SetArrayItem,			// (statement)	set item <expression> of array <expression> to <expression>
		
		NewType,				// (expression) new <type>
		NewArray,				// (expression)	new array of <expression> items
		GetArrayItem,			// (expression)	item <expression> of array <primitive>
		Invoke,					// (expression)	invoke <primitive>
		InvokeWith,				// (expression)	invoke <expression> with (<expression>, ..)
		Field,					// (assignable) field <argument> of <primitive>
		IsType,					// (expression) <primitive> is <type>
		IsNotType,				// (expression) <primitive> is not <type>

		Array,					// (type)		array
		String,					// (type)		string
		Integer,				// (type)		integer
		Float,					// (type)		float
		Symbol,					// (type)		symbol
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
		PredefinedSymbolType		type = PredefinedSymbolType::Custom;
	};

	/*************************************************************
	Symbol Stack
	*************************************************************/

	class GrammarStackItem
	{
	public:
		typedef shared_ptr<GrammarFragment>			Ptr;
		typedef vector<Ptr>							List;
		
		GrammarSymbol::List			typeSymbols;			// all symbols for a legal <type>
		GrammarSymbol::List			expressionSymbols;		// all symbols for a legal <primitive>
		GrammarSymbol::List			statementSymbols;		// all symbols for a legal <statement>
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

	class ArgumentExpression : public Expression
	{
	public:
		CodeToken::List				token;
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