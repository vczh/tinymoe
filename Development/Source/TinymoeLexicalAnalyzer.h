#ifndef VCZH_TINYMOELEXICALANALYZER
#define VCZH_TINYMOELEXICALANALYZER

#include "TinymoeSTL.h"

namespace tinymoe
{
	enum class CodeTokenType
	{
		Integer,
		Float,
		String,
		Identifier,

		Module,				// module declaration
		Using,				// import module declaration
		Phrase,				// phrase declaration
		Sentence,			// sentence declaration
		Block,				// block declaration
		Symbol,				// symbol declaration
		Type,				// type declaration

		CPS,				// continuation decoration
		Category,			// block category definition
		Expression,			// expression argument
		Argument,			// argument argument

		End,				// end of an entity

		OpenBracket,		// (
		CloseBracket,		// )
		Comma,				// ,
		Colon,				// :
		Concat,				// &
		Add,				// +
		Sub,				// -
		Mul,				// *
		Div,				// /
		LT,					// <
		GT,					// >
		LE,					// <=
		GE,					// >=
		EQ,					// =
		NE,					// <>
		And,				// and
		Or,					// or
		Not,				// not

		Unknown,
	};

	struct CodeToken
	{
		typedef vector<CodeToken>				List;

		CodeTokenType					type;
		int								row;
		int								column;
		string							value;
	};

	struct CodeError
	{
		typedef vector<CodeError>				List;

		CodeToken						begin;
		CodeToken						end;
		string							message;
	};

	struct CodeLine
	{
		typedef vector<shared_ptr<CodeLine>>	List;

		CodeToken::List					tokens;
	};

	struct CodeFile
	{
		CodeLine::List					lines;

		static shared_ptr<CodeFile>		Parse(const string& code, CodeError::List& errors);
	};
}

#endif