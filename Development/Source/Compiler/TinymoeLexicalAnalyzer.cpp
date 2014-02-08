#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
	namespace compiler
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
			case CodeTokenType::Assignable:
			case CodeTokenType::List:
			case CodeTokenType::End:
			case CodeTokenType::And:
			case CodeTokenType::Or:
			case CodeTokenType::Not:
				return true;
			default:
				return false;
			}
		}

		string_t CodeToken::EscapeString(string_t value)
		{
			stringstream_t ss;
			for (auto c : value)
			{
				switch (c)
				{
				case T('\n'):
					ss << T("\\n");
					break;
				case T('\r'):
					ss << T("\\r");
					break;
				case T('\t'):
					ss << T("\\t");
					break;
				case T('\\'):
					ss << T("\\\\");
					break;
				case T('\"'):
					ss << T("\\\"");
					break;
				default:
					ss << c;
				}
			}
			return ss.str();
		}

		string_t CodeToken::UnescapeString(string_t str)
		{
			stringstream_t ss;
			bool escaping = false;
			for (auto c : str)
			{
				if (escaping)
				{
					escaping = false;
					switch (c)
					{
					case T('n'):
						ss << T('\n');
						break;
					case T('r'):
						ss << T('\r');
						break;
					case T('t'):
						ss << T('\t');
						break;
					default:
						ss << c;
					}
				}
				else
				{
					if (c == T('\\'))
					{
						escaping = true;
					}
					else
					{
						ss << c;
					}
				}
			}
			return ss.str();
		}

		/*************************************************************
		CodeFile
		*************************************************************/

		CodeFile::Ptr CodeFile::Parse(const string_t& code, int codeIndex, CodeError::List& errors)
		{
			auto codeFile = make_shared<CodeFile>();
			enum class State
			{
				Begin,
				InPreComment,
				InComment,
				InInteger,
				InFloat,
				InString,
				InStringEscaping,
				InIdentifier,
			};
			const char_t* reading = code.c_str();
			const char_t* begin = nullptr;
			const char_t* rowBegin = reading;
			int rowNumber = 1;
			State state = State::Begin;

			auto AddToken = [&](int length, CodeTokenType type)
			{
				if (type == CodeTokenType::Comment)
				{
					return;
				}
				auto tokenBegin = begin ? begin : reading;
				string_t value(tokenBegin, tokenBegin + length);

				switch (type)
				{
				case CodeTokenType::Identifier:
					type =
						value == T("module") ? CodeTokenType::Module :
						value == T("using") ? CodeTokenType::Using :
						value == T("phrase") ? CodeTokenType::Phrase :
						value == T("sentence") ? CodeTokenType::Sentence :
						value == T("block") ? CodeTokenType::Block :
						value == T("symbol") ? CodeTokenType::Symbol :
						value == T("type") ? CodeTokenType::Type :
						value == T("cps") ? CodeTokenType::CPS :
						value == T("category") ? CodeTokenType::Category :
						value == T("expression") ? CodeTokenType::Expression :
						value == T("argument") ? CodeTokenType::Argument :
						value == T("assignable") ? CodeTokenType::Assignable :
						value == T("list") ? CodeTokenType::List :
						value == T("end") ? CodeTokenType::End :
						value == T("and") ? CodeTokenType::And :
						value == T("or") ? CodeTokenType::Or :
						value == T("not") ? CodeTokenType::Not :
						CodeTokenType::Identifier;
					break;
				case CodeTokenType::String:
					value = CodeToken::UnescapeString(value);
					break;
				}

				CodeToken token;
				token.type = type;
				token.row = rowNumber;
				token.column = tokenBegin - rowBegin + 1;
				token.value = value;
				token.codeIndex = codeIndex;

				int lineCount = codeFile->lines.size();
				auto lastLine = lineCount ? codeFile->lines[lineCount - 1] : nullptr;
				if (!lastLine || lastLine->tokens[0].row != rowNumber)
				{
					lastLine = make_shared<CodeLine>();
					codeFile->lines.push_back(lastLine);
				}

				lastLine->tokens.push_back(token);
			};

			auto AddError = [&](int length, const string_t& message)
			{
				auto tokenBegin = begin ? begin : reading;
				string_t value(tokenBegin, tokenBegin + length);
				CodeToken token;
				token.type = CodeTokenType::Unknown;
				token.row = rowNumber;
				token.column = tokenBegin - rowBegin + 1;
				token.value = value;
				token.codeIndex = codeIndex;

				CodeError error =
				{
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
					case T('('):
						AddToken(1, CodeTokenType::OpenBracket);
						break;
					case T(')'):
						AddToken(1, CodeTokenType::CloseBracket);
						break;
					case T(','):
						AddToken(1, CodeTokenType::Comma);
						break;
					case T(':'):
						AddToken(1, CodeTokenType::Colon);
						break;
					case T('&'):
						AddToken(1, CodeTokenType::Concat);
						break;
					case T('+'):
						AddToken(1, CodeTokenType::Add);
						break;
					case T('-'):
						begin = reading;
						state = State::InPreComment;
						break;
					case T('*'):
						AddToken(1, CodeTokenType::Mul);
						break;
					case T('/'):
						AddToken(1, CodeTokenType::Div);
						break;
					case T('\\'):
						AddToken(1, CodeTokenType::IntDiv);
						break;
					case T('%'):
						AddToken(1, CodeTokenType::Mod);
						break;
					case T('<'):
						switch (reading[1])
						{
						case T('='):
							AddToken(2, CodeTokenType::LE);
							reading++;
							break;
						case T('>'):
							AddToken(2, CodeTokenType::NE);
							reading++;
							break;
						default:
							AddToken(1, CodeTokenType::LT);
						}
						break;
					case T('>'):
						switch (reading[1])
						{
						case T('='):
							AddToken(2, CodeTokenType::GE);
							reading++;
							break;
						default:
							AddToken(1, CodeTokenType::GT);
						}
						break;
					case T('='):
						AddToken(1, CodeTokenType::EQ);
						break;
					case T(' '):case T('\t'):case T('\r'):
						break;
					case T('\n'):
						rowNumber++;
						rowBegin = reading + 1;
						break;
					case T('"'):
						begin = reading;
						state = State::InString;
						break;
					default:
						if (T('0') <= c && c <= T('9'))
						{
							begin = reading;
							state = State::InInteger;
						}
						else if (T('a') <= c && c <= T('z') || T('A') <= c && c <= T('Z') || c == T('_'))
						{
							begin = reading;
							state = State::InIdentifier;
						}
						else
						{
							AddError(1, T("Unknown character: \"") + string_t(reading, reading + 1) + T("\"."));
						}
					}
					break;
				case State::InPreComment:
					switch (c)
					{
					case T('-'):
						state = State::InComment;
						break;
					default:
						AddToken(reading - begin, CodeTokenType::Sub);
						state = State::Begin;
						begin = nullptr;
						reading--;
					}
					break;
				case State::InComment:
					switch (c)
					{
					case T('\n'):
						AddToken(reading - begin, CodeTokenType::Comment);
						state = State::Begin;
						begin = nullptr;
						reading--;
						break;
					}
					break;
				case State::InInteger:
					if (T('0') <= c && c <= T('9'))
					{
						// stay still
					}
					else if (c == T('.') && T('0') <= reading[1] && reading[1] <= T('9'))
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
					if (T('0') <= c && c <= T('9'))
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
					case T('\"'):
						begin++;
						AddToken(reading - begin, CodeTokenType::String);
						state = State::Begin;
						begin = nullptr;
						break;
					case T('\\'):
						state = State::InStringEscaping;
						break;
					case T('\n'):
						AddError(reading - begin, T("String literal should be single-lined."));
						state = State::Begin;
						begin = nullptr;
						reading--;
						break;
					}
					break;
				case State::InStringEscaping:
					switch (c)
					{
					case T('\n'):
						AddError(reading - begin, T("String literal should be single-lined."));
						state = State::Begin;
						begin = nullptr;
						reading--;
						break;
					default:
						state = State::InString;
					}
					break;
				case State::InIdentifier:
					if (T('a') <= c && c <= T('z') || T('A') <= c && c <= T('Z') || c == T('_') || c == T('.') || c == T('-'))
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
				AddError(reading - begin, T("String literal should be single-lined."));
				break;
			}

			return codeFile;
		}

		/*************************************************************
		CodeFragment
		*************************************************************/

		CodeFragment::CodeFragment()
		{
		}

		CodeFragment::~CodeFragment()
		{
		}
		/*************************************************************
		SymbolName
		*************************************************************/

		string_t SymbolName::GetName()
		{
			string_t result;
			for (auto it = identifiers.begin(); it != identifiers.end(); it++)
			{
				result += it->value;
				if (it + 1 != identifiers.end())
				{
					result += T(" ");
				}
			}
			return result;
		}

		string_t SymbolName::GetComposedName()
		{
			string_t result;
			for (auto it = identifiers.begin(); it != identifiers.end(); it++)
			{
				result += it->value;
				if (it + 1 != identifiers.end())
				{
					result += T("_");
				}
			}
			return result;
		}

		bool SymbolName::ConsumeToken(CodeToken::List::iterator& it, CodeToken::List::iterator end, CodeTokenType tokenType, const string_t& content, CodeToken ownerToken, CodeError::List& errors)
		{
			if (it == end)
			{
				CodeError error =
				{
					ownerToken,
					T("Incomplete code, \"") + content + T("\" expected."),
				};
				errors.push_back(error);
				return false;
			}

			if (it->type != tokenType)
			{
				CodeError error =
				{
					*it,
					T("\"") + content + T("\" expected but \"") + it->value + T("\" found."),
				};
				errors.push_back(error);
				return false;
			}

			it++;
			return true;
		}

		SymbolName::Ptr SymbolName::ParseToEnd(CodeToken::List::iterator it, CodeToken::List::iterator end, const string_t& ownerName, CodeToken ownerToken, CodeError::List& errors)
		{
			auto symbolName = make_shared<SymbolName>();
			while (it != end)
			{
				if (it->IsNameFragmentToken())
				{
					symbolName->identifiers.push_back(*it);
				}
				else
				{
					CodeError error =
					{
						*it,
						T("Token is not a legal name: \"") + it->value + T("\"."),
					};
					errors.push_back(error);
				}
				it++;
			}

			if (symbolName->identifiers.size() == 0)
			{
				CodeError error =
				{
					ownerToken,
					ownerName + T(" name should not be empty."),
				};
				errors.push_back(error);
			}
			return symbolName;
		}

		SymbolName::Ptr SymbolName::ParseToFarest(CodeToken::List::iterator& it, CodeToken::List::iterator end, const string_t& ownerName, CodeToken ownerToken, CodeError::List& errors)
		{
			auto symbolName = make_shared<SymbolName>();
			while (it != end)
			{
				if (it->IsNameFragmentToken())
				{
					symbolName->identifiers.push_back(*it++);
				}
				else
				{
					break;
				}
			}

			if (symbolName->identifiers.size() == 0)
			{
				CodeError error =
				{
					ownerToken,
					ownerName + T(" name should not be empty."),
				};
				errors.push_back(error);
			}
			return symbolName;
		}
	}
}