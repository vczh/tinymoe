#include "TinymoeExpressionAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	GrammarFragment
	*************************************************************/

	GrammarFragment::GrammarFragment(GrammarFragmentType _type)
		:type(_type)
	{
	}

	/*************************************************************
	GrammarSymbol
	*************************************************************/

	GrammarSymbol::GrammarSymbol(GrammarSymbolType _type, GrammarSymbolTarget _target)
		:type(_type)
		, target(_target)
	{
	}

	void GrammarSymbol::CalculateUniqueId()
	{
		uniqueId = "";
		for (auto fragment : fragments)
		{
			switch (fragment->type)
			{
			case GrammarFragmentType::Type:
				uniqueId += "<type> ";
				break;
			case GrammarFragmentType::Primitive:
				uniqueId += "<primitive> ";
				break;
			case GrammarFragmentType::Expression:
				uniqueId += "<expression> ";
				break;
			case GrammarFragmentType::List:
				uniqueId += "<list> ";
				break;
			case GrammarFragmentType::Assignable:
				uniqueId += "<assignable> ";
				break;
			case GrammarFragmentType::Argument:
				uniqueId += "<argument> ";
				break;
			case GrammarFragmentType::Name:
				for (auto name : fragment->identifiers)
				{
					uniqueId += name + " ";
				}
				break;
			}
		}
	}

	/*************************************************************
	GrammarStackItem
	*************************************************************/

	GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, const string& name)
	{
		GrammarFragment::Ptr fragment;
		if (symbol->fragments.size() == 0 || (fragment = symbol->fragments.back())->type != GrammarFragmentType::Name)
		{
			fragment = make_shared<GrammarFragment>(GrammarFragmentType::Name);
			symbol->fragments.push_back(fragment);
		}

		fragment->identifiers.push_back(name);
		return symbol;
	}

	GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, GrammarFragmentType type)
	{
		auto fragment = make_shared<GrammarFragment>(type);
		symbol->fragments.push_back(fragment);
		return symbol;
	}

	void GrammarStackItem::FillPredefinedSymbols()
	{
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Array)
			+ "array"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::String)
			+ "string"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Integer)
			+ "integer"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Float)
			+ "float"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Symbol)
			+ "symbol"
			);

		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::NewType)
			+ "new" + GrammarFragmentType::Type
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::NewArray)
			+ "new" + "array" + "of" + GrammarFragmentType::Expression + "items"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::GetArrayItem)
			+ "item" + GrammarFragmentType::Expression + "of" + "array" + GrammarFragmentType::Primitive
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::Invoke)
			+ "invoke" + GrammarFragmentType::Primitive
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::InvokeWith)
			+ "invoke" + GrammarFragmentType::Expression + "with" + GrammarFragmentType::List
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::IsType)
			+ GrammarFragmentType::Primitive + "is" + GrammarFragmentType::Type
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::IsNotType)
			+ GrammarFragmentType::Primitive + "is" + "not" + GrammarFragmentType::Type
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Primitive, GrammarSymbolTarget::GetField)
			+ "field" + GrammarFragmentType::Argument + "of" + GrammarFragmentType::Primitive
			);

		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::End)
			+ "end"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Select)
			+ "select" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Case)
			+ "case" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::TailCall)
			+ "tail" + "call" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::RedirectTo)
			+ "redirect" + "to" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Assign)
			+ "set" + GrammarFragmentType::Assignable + "to" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetArrayItem)
			+ "set" + "item" + GrammarFragmentType::Expression + "of" + "array" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetField)
			+ "set" + "field" + GrammarFragmentType::Argument + "of" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Expression
			);
	}

	/*************************************************************
	GrammarStack
	*************************************************************/

	void GrammarStack::Push(GrammarStackItem::Ptr stackItem)
	{
		for (auto symbol : stackItem->symbols)
		{
			symbol->CalculateUniqueId();
		}

		stackItems.push_back(stackItem);
		for (auto symbol : stackItem->symbols)
		{
			availableSymbols.insert(make_pair(symbol->uniqueId, symbol));
		}
	}

	GrammarStackItem::Ptr GrammarStack::Pop()
	{
		auto stackItem = stackItems.back();
		stackItems.pop_back();

		for (auto symbol : stackItem->symbols)
		{
			auto begin = availableSymbols.lower_bound(symbol->uniqueId);
			auto end = availableSymbols.upper_bound(symbol->uniqueId);
			auto it = find_if(begin, end, [symbol](pair<string, GrammarSymbol::Ptr> x){return x.second == symbol; });
			while (it != availableSymbols.end())
			{
				if (it->second == symbol) break;
				it++;
			}
			availableSymbols.erase(it);
		}

		return stackItem;
	}
}