#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{

	/*************************************************************
	CodeToken
	*************************************************************/

	bool CodeToken::IsNameFragmentToken()
	{
		switch (type)
		{
		case CodeTokenType::Identifier:
		case CodeTokenType::Module:
		case CodeTokenType::Using:
		case CodeTokenType::Phrase:
		case CodeTokenType::Sentence:
		case CodeTokenType::Block:
		case CodeTokenType::Symbol:
		case CodeTokenType::Type:
		case CodeTokenType::CPS:
		case CodeTokenType::Category:
		case CodeTokenType::Expression:
		case CodeTokenType::Argument:
		case CodeTokenType::End:
		case CodeTokenType::And:
		case CodeTokenType::Or:
		case CodeTokenType::Not:
			return true;
		default:
			return false;
		}
	}

	/*************************************************************
	CodeFile
	*************************************************************/

	CodeFile::Ptr CodeFile::Parse(const string& code, CodeError::List& errors)
	{
		auto codeFile = make_shared<CodeFile>();
		enum class State
		{
			Begin,
			InInteger,
			InFloat,
			InString,
			InStringEscaping,
			InIdentifier,
		};
		const char* reading = code.c_str();
		const char* begin = nullptr;
		const char* rowBegin = reading;
		int rowNumber = 1;
		State state = State::Begin;

		auto AddToken = [&](int length, CodeTokenType type)
		{
			auto tokenBegin = begin ? begin : reading;
			string value(tokenBegin, tokenBegin + length);

			switch (type)
			{
			case CodeTokenType::Identifier:
				type =
					value == "module" ? CodeTokenType::Module :
					value == "using" ? CodeTokenType::Using :
					value == "phrase" ? CodeTokenType::Phrase :
					value == "sentence" ? CodeTokenType::Sentence :
					value == "block" ? CodeTokenType::Block :
					value == "symbol" ? CodeTokenType::Symbol :
					value == "type" ? CodeTokenType::Type :
					value == "cps" ? CodeTokenType::CPS :
					value == "category" ? CodeTokenType::Category :
					value == "expression" ? CodeTokenType::Expression :
					value == "argument" ? CodeTokenType::Argument :
					value == "end" ? CodeTokenType::End :
					value == "and" ? CodeTokenType::And :
					value == "or" ? CodeTokenType::Or :
					value == "not" ? CodeTokenType::Not :
					CodeTokenType::Identifier;
				break;
			case CodeTokenType::String:
				{
					stringstream ss;
					bool escaping = false;
					for (auto c : value)
					{
						if (escaping)
						{
							escaping = false;
							switch (c)
							{
							case 'n':
								ss << '\n';
								break;
							case 'r':
								ss << '\r';
								break;
							case 't':
								ss << '\t';
								break;
							default:
								ss << c;
							}
						}
						else
						{
							if (c == '\\')
							{
								escaping = true;
							}
							else
							{
								ss << c;
							}
						}
					}
					value = ss.str();
				}
				break;
			}

			CodeToken token =
			{
				type,
				rowNumber,
				tokenBegin - rowBegin + 1,
				value,
			};

			int lineCount = codeFile->lines.size();
			auto lastLine = lineCount ? codeFile->lines[lineCount - 1] : nullptr;
			if (!lastLine || lastLine->tokens[0].row != rowNumber)
			{
				lastLine = make_shared<CodeLine>();
				codeFile->lines.push_back(lastLine);
			}

			lastLine->tokens.push_back(token);
		};

		auto AddError = [&](int length, const string& message)
		{
			auto tokenBegin = begin ? begin : reading;
			string value(tokenBegin, tokenBegin + length);
			CodeToken token =
			{
				CodeTokenType::Unknown,
				rowNumber,
				tokenBegin - rowBegin + 1,
				value,
			};
			CodeError error =
			{
				token,
				token,
				message,
			};
			errors.push_back(error);
		};

		while (auto c = *reading)
		{
			switch (state)
			{
			case State::Begin:
				switch (c)
				{
				case '(':
					AddToken(1, CodeTokenType::OpenBracket);
					break;
				case ')':
					AddToken(1, CodeTokenType::CloseBracket);
					break;
				case ',':
					AddToken(1, CodeTokenType::Comma);
					break;
				case ':':
					AddToken(1, CodeTokenType::Colon);
					break;
				case '&':
					AddToken(1, CodeTokenType::Concat);
					break;
				case '+':
					AddToken(1, CodeTokenType::Add);
					break;
				case '-':
					AddToken(1, CodeTokenType::Sub);
					break;
				case '*':
					AddToken(1, CodeTokenType::Mul);
					break;
				case '/':
					AddToken(1, CodeTokenType::Div);
					break;
				case '<':
					switch (reading[1])
					{
					case '=':
						AddToken(2, CodeTokenType::LE);
						reading++;
						break;
					case '>':
						AddToken(2, CodeTokenType::NE);
						reading++;
						break;
					default:
						AddToken(1, CodeTokenType::LT);
					}
					break;
				case '>':
					switch (reading[1])
					{
					case '=':
						AddToken(2, CodeTokenType::GE);
						reading++;
						break;
					default:
						AddToken(1, CodeTokenType::GT);
					}
					break;
				case '=':
					AddToken(1, CodeTokenType::EQ);
					break;
				case ' ':case '\t':case '\r':
					break;
				case '\n':
					rowNumber++;
					rowBegin = reading + 1;
					break;
				case '\"':
					begin = reading;
					state = State::InString;
					break;
				default:
					if ('0' <= c && c <= '9')
					{
						begin = reading;
						state = State::InInteger;
					}
					else if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_')
					{
						begin = reading;
						state = State::InIdentifier;
					}
					else
					{
						AddError(1, "Unknown character: \"" + string(reading, reading + 1) + "\".");
					}
				}
				break;
			case State::InInteger:
				if ('0' <= c && c <= '9')
				{
					// stay still
				}
				else if (c == '.' && '0' <= reading[1] && reading[1] <= '9')
				{
					state = State::InFloat;
				}
				else
				{
					AddToken(reading - begin, CodeTokenType::Integer);
					state = State::Begin;
					reading--;
					begin = nullptr;
				}
				break;
			case State::InFloat:
				if ('0' <= c && c <= '9')
				{
					// stay still
				}
				else
				{
					AddToken(reading - begin, CodeTokenType::Float);
					state = State::Begin;
					reading--;
					begin = nullptr;
				}
				break;
			case State::InString:
				switch (c)
				{
				case '\"':
					begin++;
					AddToken(reading - begin, CodeTokenType::String);
					state = State::Begin;
					begin = nullptr;
					break;
				case '\\':
					state = State::InStringEscaping;
					break;
				case '\n':
					AddError(reading - begin, "String literal should be single-lined.");
					state = State::Begin;
					begin = nullptr;
					reading--;
					break;
				}
				break;
			case State::InStringEscaping:
				switch (c)
				{
				case '\n':
					AddError(reading - begin, "String literal should be single-lined.");
					state = State::Begin;
					begin = nullptr;
					reading--;
					break;
				default:
					state = State::InString;
				}
				break;
			case State::InIdentifier:
				if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_' || c == '.' || c == '-')
				{
					// stay still
				}
				else
				{
					AddToken(reading - begin, CodeTokenType::Identifier);
					state = State::Begin;
					reading--;
					begin = nullptr;
				}
				break;
			}
			reading++;
		}

		switch (state)
		{
		case State::InInteger:
			AddToken(reading - begin, CodeTokenType::Integer);
			break;
		case State::InFloat:
			AddToken(reading - begin, CodeTokenType::Float);
			break;
		case State::InIdentifier:
			AddToken(reading - begin, CodeTokenType::Identifier);
			break;
		case State::InString:
		case State::InStringEscaping:
			AddError(reading - begin, "String literal should be single-lined.");
			break;
		}

		return codeFile;
	}
}