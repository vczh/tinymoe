#define _CRT_SECURE_NO_WARNINGS
#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;
using namespace tinymoe::ast;

class CSharpNameResolver
{
private:
	map<AstDeclaration*, AstDeclaration*>			declScopes;
	map<AstDeclaration*, string_t>					resolvedNames;
	map<pair<AstDeclaration*, string_t>, int>		scopedAppearCount;
public:
	void Scope(AstDeclaration* decl, AstDeclaration* scope)
	{
		declScopes.insert(make_pair(decl, scope));
	}

	string_t Resolve(string_t name, AstDeclaration* scope)
	{
		auto scopeName = make_pair(scope, name);
		auto itappear = scopedAppearCount.find(scopeName);
		if (itappear == scopedAppearCount.end())
		{
			scopedAppearCount.insert(make_pair(scopeName, 1));
			return name;
		}
		else
		{
			int counter = ++itappear->second;
			char_t buffer[20] = { 0 };
#ifdef _UNICODE_TINYMOE
			_itow(counter, buffer, 10);
#else
			_itoa(counter, buffer, 10);
#endif
			return name + string_t(T("_x")) + buffer;
		}
	}

	string_t Resolve(AstDeclaration* decl)
	{
		auto it = resolvedNames.find(decl);
		if (it == resolvedNames.end())
		{
			stringstream_t ss;
			for (auto c : decl->composedName)
			{
				if (T('a') <= c && c <= T('z') || T('A') <= c && c <= T('Z') || T('0') <= c && c <= T('9') || c == T('_'))
				{
					ss << c;
				}
				else
				{
					ss << T('_');
				}
			}
			string_t name = ss.str();
			auto scope = declScopes.find(decl)->second;
			string_t declName = Resolve(name, scope);
			resolvedNames.insert(make_pair(decl, declName));
			return declName;
		}
		else
		{
			return it->second;
		}
	}
};

class CSharpTypeCodegen :public AstTypeVisitor
{
public:
	CSharpNameResolver&		resolver;
	string_t				result;

	CSharpTypeCodegen(CSharpNameResolver& _resolver)
		:resolver(_resolver)
	{
	}

	void Visit(AstPredefinedType* node)override
	{
		switch (node->typeName)
		{
		case AstPredefinedTypeName::Object:
			result = T("TinymoeObject");
			break;
		case AstPredefinedTypeName::Symbol:
			result = T("TinymoeSymbol");
			break;
		case AstPredefinedTypeName::Array:
			result = T("TinymoeArray");
			break;
		case AstPredefinedTypeName::Boolean:
			result = T("TinymoeBoolean");
			break;
		case AstPredefinedTypeName::Integer:
			result = T("TinymoeInteger");
			break;
		case AstPredefinedTypeName::Float:
			result = T("TinymoeFloat");
			break;
		case AstPredefinedTypeName::String:
			result = T("TinymoeString");
			break;
		case AstPredefinedTypeName::Function:
			result = T("TinymoeFunction");
			break;
		}
	}

	void Visit(AstReferenceType* node)override
	{
		result = resolver.Resolve(node->typeDeclaration.lock().get());
	}

	static string_t ToString(AstType::Ptr type, CSharpNameResolver& _resolver)
	{
		CSharpTypeCodegen codegen(_resolver);
		type->Accept(&codegen);
		return codegen.result;
	}
};

string_t FunctionToName(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	return resolver.Resolve(decl);
}

string_t FunctionToTypedName(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	auto methodName = FunctionToName(resolver, decl);
	return (decl->ownerType ? CSharpTypeCodegen::ToString(decl->ownerType, resolver) + T("__") : T("")) + methodName;
}

string_t FunctionToValue(CSharpNameResolver& resolver, AstFunctionDeclaration* decl, AstDeclaration* scope)
{
	stringstream_t ss;
	string_t argumentName = resolver.Resolve(T("__args__"), scope);
	ss << T("new TinymoeFunction(") << argumentName << T(" => ") << FunctionToTypedName(resolver, decl) << T("(");
	for (auto it = decl->arguments.begin(); it != decl->arguments.end(); it++)
	{
		ss << argumentName << T("[") << it - decl->arguments.begin() << T("]");
		if (it + 1 == decl->arguments.end())
		{
			ss << T("))");
		}
		else
		{
			ss << T(", ");
		}
	}
	return ss.str();
}

void PrintExpression(AstExpression::Ptr expression, AstDeclaration* scope, CSharpNameResolver& resolver, ostream_t& o, string_t prefix);
void PrintStatement(AstStatement::Ptr statement, AstDeclaration* scope, CSharpNameResolver& resolver, ostream_t& o, string_t prefix, bool block);

class CSharpExpressionCodegen :public AstExpressionVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream_t&				o;
	string_t				prefix;
	AstDeclaration*			scope;

	CSharpExpressionCodegen(CSharpNameResolver& _resolver, ostream_t& _o, string_t _prefix, AstDeclaration* _scope)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
		, scope(_scope)
	{
	}

	void PrintExpressionList(AstExpression::List& exprs)
	{
		for (auto it = exprs.begin(); it != exprs.end(); it++)
		{
			PrintExpression(*it, scope, resolver, o, prefix);
			if (it + 1 != exprs.end())
			{
				o << T(", ");
			}
		}
	}

	void Visit(AstLiteralExpression* node)
	{
		switch (node->literalName)
		{
		case AstLiteralName::Null:
			o << T("null");
			break;
		case AstLiteralName::True:
			o << T("new TinymoeBoolean(true)");
			break;
		case AstLiteralName::False:
			o << T("new TinymoeBoolean(false)");
			break;
		}
	}

	void Visit(AstIntegerExpression* node)
	{
		o << T("new TinymoeInteger(") << node->value << T(")");
	}

	void Visit(AstFloatExpression* node)
	{
		o << T("new TinymoeFloat(") << node->value << T(")");
	}

	void Visit(AstStringExpression* node)
	{
		o << T("new TinymoeString(\"") << node->value << T("\")");
	}

	void Visit(AstExternalSymbolExpression* node)
	{
		o << T("GetExternalFunction(\"") << node->name << T("\")");
	}

	void Visit(AstReferenceExpression* node)
	{
		auto decl = node->reference.lock();
		if (auto func = dynamic_pointer_cast<AstFunctionDeclaration>(decl))
		{
			o << FunctionToValue(resolver, func.get(), scope);
		}
		else
		{
			o << resolver.Resolve(decl.get());
		}
	}

	void Visit(AstNewTypeExpression* node)
	{
		o << T("new ") << CSharpTypeCodegen::ToString(node->type, resolver) << T("().SetFields(new TinymoeObject[] {");
		PrintExpressionList(node->fields);
		o << T("}).FinishConstruction()");
	}

	void Visit(AstTestTypeExpression* node)
	{
		o << T("new TinymoeBoolean(");
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << T(" is ") << CSharpTypeCodegen::ToString(node->type, resolver) << T(")");
	}

	void Visit(AstNewArrayExpression* node)
	{
		o << T("new TinymoeArray(((TinymoeInteger)CastToInteger(");
		PrintExpression(node->length, scope, resolver, o, prefix);
		o << T(")).Value)");
	}

	void Visit(AstNewArrayLiteralExpression* node)
	{
		o << T("new TinymoeArray(new TinymoeObject[] {");
		PrintExpressionList(node->elements);
		o << T("})");
	}

	void Visit(AstArrayLengthExpression* node)
	{
		o << T("ArrayLength(");
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << T(")");
	}

	void Visit(AstArrayAccessExpression* node)
	{
		o << T("ArrayGet(");
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << T(", ");
		PrintExpression(node->index, scope, resolver, o, prefix);
		o << T(")");
	}

	void Visit(AstFieldAccessExpression* node)
	{
		o << T("GetField(");
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << T(", \"") << node->composedFieldName << T("\")");
	}

	void Visit(AstInvokeExpression* node)
	{
		AstFunctionDeclaration::Ptr func;
		if (auto ref = dynamic_pointer_cast<AstReferenceExpression>(node->function))
		{
			func = dynamic_pointer_cast<AstFunctionDeclaration>(ref->reference.lock());
		}

		if (func)
		{
			o << FunctionToName(resolver, func.get()) << T("(") << endl;
		}
		else
		{
			o << T("Invoke(");
			PrintExpression(node->function, scope, resolver, o, prefix);
			o << T(", new TinymoeObject[] {") << endl;
		}
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			o << prefix << T("\t");
			PrintExpression(*it, scope, resolver, o, prefix + T("\t"));
			if (it + 1 != node->arguments.end())
			{
				o << T(",") << endl;
			}
		}
		if (func)
		{
			o << endl << prefix << T("\t)");
		}
		else
		{
			o << endl << prefix << T("\t})");
		}
	}

	void Visit(AstLambdaExpression* node)
	{
		string_t argumentName = resolver.Resolve(T("__args__"), scope);
		o << T("new TinymoeFunction(") << argumentName << T(" => ") << endl;
		o << prefix << T("{") << endl;
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			resolver.Scope(it->get(), scope);
			o << prefix << T("\tTinymoeObject ") << resolver.Resolve(it->get()) << T(" = ") << argumentName << T("[") << it - node->arguments.begin() << T("];") << endl;
		}
		PrintStatement(node->statement, scope, resolver, o, prefix + T("\t"), true);
		o << endl << prefix << T("})");
	}
};

class CSharpSetExpressionCodegen :public AstExpressionVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream_t&				o;
	string_t					prefix;
	AstDeclaration*			scope;
	AstExpression::Ptr		value;

	CSharpSetExpressionCodegen(CSharpNameResolver& _resolver, ostream_t& _o, string_t _prefix, AstDeclaration* _scope, AstExpression::Ptr _value)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
		, scope(_scope)
		, value(_value)
	{
	}

	void Visit(AstLiteralExpression* node)
	{
		throw 0;
	}

	void Visit(AstIntegerExpression* node)
	{
		throw 0;
	}

	void Visit(AstFloatExpression* node)
	{
		throw 0;
	}

	void Visit(AstStringExpression* node)
	{
		throw 0;
	}

	void Visit(AstExternalSymbolExpression* node)
	{
		throw 0;
	}

	void Visit(AstReferenceExpression* node)
	{
		auto decl = node->reference.lock();
		if (auto func = dynamic_pointer_cast<AstFunctionDeclaration>(decl))
		{
			o << FunctionToValue(resolver, func.get(), scope);
		}
		else
		{
			o << resolver.Resolve(decl.get());
		}
		o << T(" = ");
		PrintExpression(value, scope, resolver, o, prefix);
		o << T(";");
	}

	void Visit(AstNewTypeExpression* node)
	{
		throw 0;
	}

	void Visit(AstTestTypeExpression* node)
	{
		throw 0;
	}

	void Visit(AstNewArrayExpression* node)
	{
		throw 0;
	}

	void Visit(AstNewArrayLiteralExpression* node)
	{
		throw 0;
	}

	void Visit(AstArrayLengthExpression* node)
	{
		throw 0;
	}

	void Visit(AstArrayAccessExpression* node)
	{
		o << T("ArraySet(");
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << T(", ");
		PrintExpression(node->index, scope, resolver, o, prefix);
		o << T(", ");
		PrintExpression(value, scope, resolver, o, prefix);
		o << T(");");
	}

	void Visit(AstFieldAccessExpression* node)
	{
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << T(".SetField(\"") << node->composedFieldName << T("\", ");
		PrintExpression(value, scope, resolver, o, prefix);
		o << T(");");
	}

	void Visit(AstInvokeExpression* node)
	{
		throw 0;
	}

	void Visit(AstLambdaExpression* node)
	{
		throw 0;
	}
};

class CSharpStatementCodegen :public AstStatementVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream_t&				o;
	string_t					prefix;
	bool					block;
	AstDeclaration*			scope;

	CSharpStatementCodegen(CSharpNameResolver& _resolver, ostream_t& _o, string_t _prefix, bool _block, AstDeclaration* _scope)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
		, block(_block)
		, scope(_scope)
	{
	}

	void Visit(AstBlockStatement* node)
	{
		if (block)
		{
			for (auto it = node->statements.begin(); it != node->statements.end(); it++)
			{
				PrintStatement(*it, scope, resolver, o, prefix, false);
				if (it + 1 != node->statements.end())
				{
					o << endl;
				}
			}
		}
		else
		{
			o << prefix << T("{") << endl;
			for (auto stat : node->statements)
			{
				PrintStatement(stat, scope, resolver, o, prefix + T("\t"), false);
				o << endl;
			}
			o << prefix << T("}");
		}
	}

	void Visit(AstExpressionStatement* node)
	{
		o << prefix;
		PrintExpression(node->expression, scope, resolver, o, prefix);
		o << T(";");
	}

	void Visit(AstDeclarationStatement* node)
	{
		resolver.Scope(node->declaration.get(), scope);
		o << prefix << T("TinymoeObject ") << resolver.Resolve(node->declaration.get()) << T(" = null;");
	}

	void Visit(AstAssignmentStatement* node)
	{
		o << prefix;
		CSharpSetExpressionCodegen codegen(resolver, o, prefix, scope, node->value);
		node->target->Accept(&codegen);
	}

	void Visit(AstIfStatement* node)
	{
		AstIfStatement* current = node;
		o << prefix;
		while (true)
		{
			o << T("if (((TinymoeBoolean)CastToBoolean(");
			PrintExpression(current->condition, scope, resolver, o, prefix);
			o << T(")).Value)") << endl;
			o << prefix << T("{") << endl;
			PrintStatement(current->trueBranch, scope, resolver, o, prefix + T("\t"), true);
			o << endl << prefix << T("}");

			auto nextCurrent = dynamic_cast<AstIfStatement*>(current->falseBranch.get());
			if (!nextCurrent) break;
			o << endl << prefix << T("else ");
			current = nextCurrent;
		}

		if (current->falseBranch)
		{
			o << endl << prefix << T("else") << endl;
			o << prefix << T("{") << endl;
			PrintStatement(current->falseBranch, scope, resolver, o, prefix + T("\t"), true);
			o << endl << prefix << T("}");
		}
	}
};

void PrintExpression(AstExpression::Ptr expression, AstDeclaration* scope, CSharpNameResolver& resolver, ostream_t& o, string_t prefix)
{
	CSharpExpressionCodegen codegen(resolver, o, prefix, scope);
	expression->Accept(&codegen);
}

void PrintStatement(AstStatement::Ptr statement, AstDeclaration* scope, CSharpNameResolver& resolver, ostream_t& o, string_t prefix, bool block)
{
	CSharpStatementCodegen codegen(resolver, o, prefix, block, scope);
	statement->Accept(&codegen);
}

class CSharpDeclarationCodegen :public AstDeclarationVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream_t&				o;
	string_t					prefix;

	CSharpDeclarationCodegen(CSharpNameResolver& _resolver, ostream_t& _o, string_t _prefix)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
	{
	}

	void Visit(AstSymbolDeclaration* node)override
	{
		o << prefix << T("public readonly TinymoeObject ") << resolver.Resolve(node) << T(" = new TinymoeSymbol(\"") << resolver.Resolve(node) << T("\");") << endl << endl;
	}

	void Visit(AstTypeDeclaration* node)override
	{
		o << prefix << T("public class ") << resolver.Resolve(node);
		if (node->baseType.expired())
		{
			o << T(" : TinymoeObject") << endl;
		}
		else
		{
			o << T(" : ") << CSharpTypeCodegen::ToString(node->baseType.lock(), resolver) << endl;
		}
		o << prefix << T("{") << endl;
		o << prefix << T("\tpublic ") << resolver.Resolve(node) << T("()") << endl;
		o << prefix << T("\t{") << endl;
		for (auto field : node->fields)
		{
			resolver.Scope(field.get(), node);
			o << prefix << T("\t\tSetField(\"") << resolver.Resolve(field.get()) << T("\", null);") << endl;
		}
		o << prefix << T("\t}") << endl;
		o << prefix << T("}") << endl << endl;
	}

	void Visit(AstFunctionDeclaration* node)override
	{
		o << prefix << T("public void ") << FunctionToTypedName(resolver, node) << T("(");
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			resolver.Scope(it->get(), node);
			o << T("TinymoeObject ") << resolver.Resolve(it->get());
			if (it + 1 == node->arguments.end())
			{
				o << T(")") << endl;
			}
			else
			{
				o << T(", ");
			}
		}
		o << prefix << T("{") << endl;
		PrintStatement(node->statement, node, resolver, o, prefix + T("\t"), true);
		o << endl;
		o << prefix << T("}") << endl << endl;
	}
};

class CSharpExtensionDeclarationCodegen :public AstDeclarationVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream_t&				o;
	string_t					prefix;

	CSharpExtensionDeclarationCodegen(CSharpNameResolver& _resolver, ostream_t& _o, string_t _prefix)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
	{
	}

	void Visit(AstSymbolDeclaration* node)override
	{
	}

	void Visit(AstTypeDeclaration* node)override
	{
	}

	void Visit(AstFunctionDeclaration* node)override
	{
		if (node->ownerType)
		{
			auto typeName = CSharpTypeCodegen::ToString(node->ownerType, resolver);
			auto methodName = node->composedName;
			auto targetName = FunctionToValue(resolver, node, nullptr);
			o << prefix << T("SetExtension(") << endl;
			o << prefix << T("\ttypeof(") << typeName << T("),") << endl;
			o << prefix << T("\t\"") << methodName << T("\",") << endl;
			o << prefix << T("\t") << targetName << endl;
			o << prefix << T("\t); ") << endl;
		}
	}
};

void GenerateCSharpCode(AstAssembly::Ptr assembly, ostream_t& o)
{
	CSharpNameResolver resolver;
	o << T("using System;") << endl;
	o << T("using System.Collections.Generic;") << endl;
	o << T("using TinymoeDotNet;") << endl;
	o << T("") << endl;
	o << T("namespace TinymoeProgramNamespace") << endl;
	o << T("{") << endl;
	o << T("\tpublic class TinymoeProgram : TinymoeOperations") << endl;
	o << T("\t{") << endl;
	{
		for (auto decl : assembly->declarations)
		{
			resolver.Scope(decl.get(), nullptr);
		}
		CSharpDeclarationCodegen codegen(resolver, o, T("\t\t"));
		for (auto decl : assembly->declarations)
		{
			decl->Accept(&codegen);
		}
	}
	o << T("\t\tpublic TinymoeProgram()") << endl;
	o << T("\t\t{") << endl;
	{
		CSharpExtensionDeclarationCodegen codegen(resolver, o, T("\t\t\t"));
		for (auto decl : assembly->declarations)
		{
			decl->Accept(&codegen);
		}
	}
	o << T("\t\t}") << endl;
	o << endl;
	{
		string_t mainName;
		for (auto decl : assembly->declarations)
		{
			if (decl->composedName.size() >= 6)
			{
				if (decl->composedName.substr(decl->composedName.size() - 6, 6) == T("::main"))
				{
					mainName = resolver.Resolve(decl.get());
					break;
				}
			}
		}
		o << T("\t\tstatic void Main(string[] args)") << endl;
		o << T("\t\t{") << endl;
		o << T("\t\t\tvar program = new TinymoeProgram();") << endl;
		o << T("\t\t\tvar continuation = new TinymoeFunction((TinymoeObject[] arguments) =>") << endl;
		o << T("\t\t\t{") << endl;
		o << T("\t\t\t});") << endl;
		o << T("\t\t\tvar trap = new TinymoeProgram.standard_library__continuation_trap();") << endl;
		o << T("\t\t\ttrap.SetField(\"continuation\", continuation);") << endl;
		o << T("\t\t\tvar state = new TinymoeProgram.standard_library__continuation_state();") << endl;
		o << T("\t\t\tstate.SetField(\"trap\", trap);") << endl;
		o << T("\t\t\tprogram.") << mainName << T("(state, continuation);") << endl;
		o << T("\t\t}") << endl;
	}
	o << T("\t}") << endl;
	o << T("}") << endl;
}