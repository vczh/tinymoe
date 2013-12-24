#ifndef VCZH_TINYMOE
#define VCZH_TINYMOE

#include <vector>
#include <memory>

using namespace std;

namespace tinymoe
{
	/*************************************************************
	Lexical Analyzer
	*************************************************************/

	enum CodeTokenType
	{
		Integer,
		Float,
		String,
		Identifier,

		Pharse,
		Sentence,
		Argument,
		Expression,
		Block,
		End,

		OpenBracket,
		CloseBracket,
		Concat,
		Add,
		Sub,
		Mul,
		Div,
		LT,
		GT,
		LE,
		GE,
		EQ,
		NE,
		And,
		Or,
		Not,
	};

	struct CodeToken
	{
		typedef vector<CodeToken>				List;

		int						row;
		int						column;
		char*					start;
		int						length;
	};

	struct CodeLine
	{
		typedef vector<shared_ptr<CodeLine>>	List;

		CodeToken::List			tokens;
	};

	struct CodeFile
	{
		CodeLine::List			lines;

		void					Parse(const string& code);
	};
}

#endif