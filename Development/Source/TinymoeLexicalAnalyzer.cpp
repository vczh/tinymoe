#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	CodeFile
	*************************************************************/

	shared_ptr<CodeFile> CodeFile::Parse(const string& code, CodeError::List& errors)
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

		auto AddToken = [=, &begin](int length, CodeTokenType type)
		{
			string value(reading, reading + length);

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
					ss >> value;
				}
				break;
			}

			CodeToken token =
			{
				type,
				rowNumber,
				reading - rowBegin,
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

		auto AddError = [=, &errors](int length, const string& message)
		{
			CodeToken token =
			{
				CodeTokenType::Unknown,
				rowNumber,
				reading - rowBegin,
				string(reading, reading + length),
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
						AddToken(1, CodeTokenType::NE);
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
						AddError(1, "Unknown character: " + string(*reading, *reading + 1) + ".");
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
					reading--;
					begin = nullptr;
				}
				break;
			case State::InString:
				switch (c)
				{
				case '\"':
					begin++;
					AddToken(reading - begin - 1, CodeTokenType::String);
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