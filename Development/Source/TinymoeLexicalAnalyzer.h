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
		Assignable,			// assignable argument
		List,				// list

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

		CodeTokenType					type = CodeTokenType::Unknown;
		int								row = -1;
		int								column = -1;
		string							value;

		bool							IsNameFragmentToken();
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
		typedef shared_ptr<CodeLine>			Ptr;
		typedef vector<Ptr>						List;

		CodeToken::List					tokens;
	};

	struct CodeFile
	{
		typedef shared_ptr<CodeFile>			Ptr;

		CodeLine::List					lines;

		static CodeFile::Ptr			Parse(const string& code, CodeError::List& errors);
	};

	class CodeFragment
	{
	public:
		CodeFragment();
		virtual ~CodeFragment();
	};
}

#endif