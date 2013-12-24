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
			CodeToken token =
			{
				type,
				rowNumber,
				reading - rowBegin,
				string(reading, reading + length),
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
				case L'(':
					AddToken(1, CodeTokenType::OpenBracket);
					break;
				case L')':
					AddToken(1, CodeTokenType::CloseBracket);
					break;
				case L',':
					AddToken(1, CodeTokenType::Comma);
					break;
				case L':':
					AddToken(1, CodeTokenType::Colon);
					break;
				case L'&':
					AddToken(1, CodeTokenType::Concat);
					break;
				case L'+':
					AddToken(1, CodeTokenType::Add);
					break;
				case L'-':
					AddToken(1, CodeTokenType::Sub);
					break;
				case L'*':
					AddToken(1, CodeTokenType::Mul);
					break;
				case L'/':
					AddToken(1, CodeTokenType::Div);
					break;
				case L'<':
					switch (reading[1])
					{
					case L'=':
						AddToken(2, CodeTokenType::LE);
						reading++;
						break;
					case L'>':
						AddToken(1, CodeTokenType::NE);
						reading++;
						break;
					default:
						AddToken(1, CodeTokenType::LT);
					}
					break;
				case L'>':
					switch (reading[1])
					{
					case L'=':
						AddToken(2, CodeTokenType::GE);
						reading++;
						break;
					default:
						AddToken(1, CodeTokenType::GT);
					}
					break;
				case L'=':
					AddToken(1, CodeTokenType::EQ);
					break;
				case L' ':case L'\t':case L'\r':
					break;
				case L'\n':
					rowNumber++;
					rowBegin = reading + 1;
					break;
				case L'\"':
					begin = reading;
					state = State::InString;
				default:
					if (L'0' <= c && c <= L'9')
					{
						begin = reading;
						state = State::InInteger;
					}
					else if (L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z' || c == L'_')
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
				if (L'0' <= c && c <= L'9')
				{
					// stay still
				}
				else if (c == L'.' && L'0' <= reading[1] && reading[1] <= L'9')
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
				if (L'0' <= c && c <= L'9')
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
				case L'\"':
					AddToken(reading - begin + 1, CodeTokenType::String);
					begin = nullptr;
					break;
				case L'\\':
					state = State::InStringEscaping;
					break;
				case L'\n':
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
				case L'\n':
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
				if (L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z' || c == L'_' || c == L'.' || c == L'-')
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