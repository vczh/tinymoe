#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;
using namespace tinymoe::ast;

class CSharpNameResolver
{
private:
	map<AstDeclaration*, AstDeclaration*>		declScopes;
	map<AstDeclaration*, string>				resolvedNames;
	map<pair<AstDeclaration*, string>, int>		scopedAppearCount;
public:
	void Scope(AstDeclaration* decl, AstDeclaration* scope)
	{
		declScopes.insert(make_pair(decl, scope));
	}

	string Resolve(string name, AstDeclaration* scope)
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
			char buffer[20] = { 0 };
			_itoa_s(counter, buffer, 10);
			return name + string("_x") + buffer;
		}
	}

	string Resolve(AstDeclaration* decl)
	{
		auto it = resolvedNames.find(decl);
		if (it == resolvedNames.end())
		{
			stringstream ss;
			for (auto c : decl->composedName)
			{
				if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_')
				{
					ss << c;
				}
				else
				{
					ss << '_';
				}
			}
			string name = ss.str();
			auto scope = declScopes.find(decl)->second;
			string declName = Resolve(name, scope);
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
	string					result;

	CSharpTypeCodegen(CSharpNameResolver& _resolver)
		:resolver(_resolver)
	{
	}

	void Visit(AstPredefinedType* node)override
	{
		switch (node->typeName)
		{
		case AstPredefinedTypeName::Object:
			result = "TinymoeObject";
			break;
		case AstPredefinedTypeName::Symbol:
			result = "TinymoeSymbol";
			break;
		case AstPredefinedTypeName::Array:
			result = "TinymoeArray";
			break;
		case AstPredefinedTypeName::Boolean:
			result = "TinymoeBoolean";
			break;
		case AstPredefinedTypeName::Integer:
			result = "TinymoeInteger";
			break;
		case AstPredefinedTypeName::Float:
			result = "TinymoeFloat";
			break;
		case AstPredefinedTypeName::String:
			result = "TinymoeString";
			break;
		case AstPredefinedTypeName::Function:
			result = "TinymoeFunction";
			break;
		}
	}

	void Visit(AstReferenceType* node)override
	{
		result = resolver.Resolve(node->typeDeclaration.lock().get());
	}

	static string ToString(AstType::Ptr type, CSharpNameResolver& _resolver)
	{
		CSharpTypeCodegen codegen(_resolver);
		type->Accept(&codegen);
		return codegen.result;
	}
};

string FunctionToName(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	return resolver.Resolve(decl);
}

string FunctionToTypedName(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	auto methodName = FunctionToName(resolver, decl);
	return (decl->ownerType ? CSharpTypeCodegen::ToString(decl->ownerType, resolver) + "__" : "") + methodName;
}

string FunctionToValue(CSharpNameResolver& resolver, AstFunctionDeclaration* decl, AstDeclaration* scope)
{
	stringstream ss;
	string argumentName = resolver.Resolve("__args__", scope);
	ss << "new TinymoeFunction(" << argumentName << " => " << FunctionToTypedName(resolver, decl) << "(";
	for (auto it = decl->arguments.begin(); it != decl->arguments.end(); it++)
	{
		ss << argumentName << "[" << it - decl->arguments.begin() << "]";
		if (it + 1 == decl->arguments.end())
		{
			ss << "))";
		}
		else
		{
			ss << ", ";
		}
	}
	return ss.str();
}

void PrintExpression(AstExpression::Ptr expression, AstDeclaration* scope, CSharpNameResolver& resolver, ostream& o, string prefix);
void PrintStatement(AstStatement::Ptr statement, AstDeclaration* scope, CSharpNameResolver& resolver, ostream& o, string prefix, bool block);

class CSharpExpressionCodegen :public AstExpressionVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream&				o;
	string					prefix;
	AstDeclaration*			scope;

	CSharpExpressionCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix, AstDeclaration* _scope)
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
				o << ", ";
			}
		}
	}

	void Visit(AstLiteralExpression* node)
	{
		switch (node->literalName)
		{
		case AstLiteralName::Null:
			o << "null";
			break;
		case AstLiteralName::True:
			o << "new TinymoeBoolean(true)";
			break;
		case AstLiteralName::False:
			o << "new TinymoeBoolean(false)";
			break;
		}
	}

	void Visit(AstIntegerExpression* node)
	{
		o << "new TinymoeInteger(" << node->value << ")";
	}

	void Visit(AstFloatExpression* node)
	{
		o << "new TinymoeFloat(" << node->value << ")";
	}

	void Visit(AstStringExpression* node)
	{
		o << "new TinymoeString(\"" << node->value << "\")";
	}

	void Visit(AstExternalSymbolExpression* node)
	{
		o << "GetExternalFunction(";
		PrintExpression(node->name, scope, resolver, o, prefix);
		o << ")";
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

	void Visit(AstUnaryExpression* node)
	{
		switch (node->op)
		{
		case AstUnaryOperator::Positive:
			o << "Positive(";
			break;
		case AstUnaryOperator::Negative:
			o << "Negative(";
			break;
		case AstUnaryOperator::Not:
			o << "Not(";
			break;
		}
		PrintExpression(node->operand, scope, resolver, o, prefix);
		o << ")";
	}

	void Visit(AstBinaryExpression* node)
	{
		switch (node->op)
		{
		case AstBinaryOperator::Concat:
			o << "Concat(";
			break;
		case AstBinaryOperator::Add:
			o << "Add(";
			break;
		case AstBinaryOperator::Sub:
			o << "Sub(";
			break;
		case AstBinaryOperator::Mul:
			o << "Mul(";
			break;
		case AstBinaryOperator::Div:
			o << "Div(";
			break;
		case AstBinaryOperator::IntDiv:
			o << "IntDiv(";
			break;
		case AstBinaryOperator::Mod:
			o << "Mod(";
			break;
		case AstBinaryOperator::LT:
			o << "LT(";
			break;
		case AstBinaryOperator::GT:
			o << "GT(";
			break;
		case AstBinaryOperator::LE:
			o << "LE(";
			break;
		case AstBinaryOperator::GE:
			o << "GE(";
			break;
		case AstBinaryOperator::EQ:
			o << "EQ(";
			break;
		case AstBinaryOperator::NE:
			o << "NE(";
			break;
		case AstBinaryOperator::And:
			o << "And(";
			break;
		case AstBinaryOperator::Or:
			o << "Or(";
			break;
		}
		PrintExpression(node->first, scope, resolver, o, prefix);
		o << ", ";
		PrintExpression(node->second, scope, resolver, o, prefix);
		o << ")";
	}

	void Visit(AstNewTypeExpression* node)
	{
		o << "new " << CSharpTypeCodegen::ToString(node->type, resolver) << "().SetFields(new TinymoeObject[] {";
		PrintExpressionList(node->fields);
		o << "})";
	}

	void Visit(AstTestTypeExpression* node)
	{
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << " is " << CSharpTypeCodegen::ToString(node->type, resolver);
	}

	void Visit(AstNewArrayExpression* node)
	{
		o << "new TinymoeArray(((TinymoeInteger)CastToInteger(";
		PrintExpression(node->length, scope, resolver, o, prefix);
		o << ")).Value)";
	}

	void Visit(AstNewArrayLiteralExpression* node)
	{
		o << "new TinymoeArray(new TinymoeObject[] {";
		PrintExpressionList(node->elements);
		o << "})";
	}

	void Visit(AstArrayLengthExpression* node)
	{
		o << "ArrayLength(";
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << ")";
	}

	void Visit(AstArrayAccessExpression* node)
	{
		o << "ArrayGet(";
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << ", ";
		PrintExpression(node->index, scope, resolver, o, prefix);
		o << ")";
	}

	void Visit(AstFieldAccessExpression* node)
	{
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << ".GetField(\"" << node->composedFieldName << "\")";
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
			o << FunctionToName(resolver, func.get()) << "(" << endl;
		}
		else
		{
			o << "Invoke(";
			PrintExpression(node->function, scope, resolver, o, prefix);
			o << ", new TinymoeObject[] {" << endl;
		}
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			o << prefix << "\t";
			PrintExpression(*it, scope, resolver, o, prefix + "\t");
			if (it + 1 != node->arguments.end())
			{
				o << "," << endl;
			}
		}
		if (func)
		{
			o << endl << prefix << "\t)";
		}
		else
		{
			o << endl << prefix << "\t})";
		}
	}

	void Visit(AstLambdaExpression* node)
	{
		string argumentName = resolver.Resolve("__args__", scope);
		o << "new TinymoeFunction(" << argumentName << " => " << endl;
		o << prefix << "{" << endl;
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			resolver.Scope(it->get(), scope);
			o << prefix << "\tTinymoeObject " << resolver.Resolve(it->get()) << " = " << argumentName << "[" << it - node->arguments.begin() << "];" << endl;
		}
		PrintStatement(node->statement, scope, resolver, o, prefix + "\t", true);
		o << endl << prefix << "})";
	}
};

class CSharpSetExpressionCodegen :public AstExpressionVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream&				o;
	string					prefix;
	AstDeclaration*			scope;
	AstExpression::Ptr		value;

	CSharpSetExpressionCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix, AstDeclaration* _scope, AstExpression::Ptr _value)
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
		o << " = ";
		PrintExpression(value, scope, resolver, o, prefix);
		o << ";";
	}

	void Visit(AstUnaryExpression* node)
	{
		throw 0;
	}

	void Visit(AstBinaryExpression* node)
	{
		throw 0;
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
		o << "ArraySet(";
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << ", ";
		PrintExpression(node->index, scope, resolver, o, prefix);
		o << ", ";
		PrintExpression(value, scope, resolver, o, prefix);
		o << ");";
	}

	void Visit(AstFieldAccessExpression* node)
	{
		PrintExpression(node->target, scope, resolver, o, prefix);
		o << ".SetField(\"" << node->composedFieldName << "\", ";
		PrintExpression(value, scope, resolver, o, prefix);
		o << ");";
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
	ostream&				o;
	string					prefix;
	bool					block;
	AstDeclaration*			scope;

	CSharpStatementCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix, bool _block, AstDeclaration* _scope)
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
			o << prefix << "{" << endl;
			for (auto stat : node->statements)
			{
				PrintStatement(stat, scope, resolver, o, prefix + "\t", false);
				o << endl;
			}
			o << prefix << "}";
		}
	}

	void Visit(AstExpressionStatement* node)
	{
		o << prefix;
		PrintExpression(node->expression, scope, resolver, o, prefix);
		o << ";";
	}

	void Visit(AstDeclarationStatement* node)
	{
		resolver.Scope(node->declaration.get(), scope);
		o << prefix << "TinymoeObject " << resolver.Resolve(node->declaration.get()) << " = null;";
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
			o << "if (((TinymoeBoolean)CastToBoolean(";
			PrintExpression(current->condition, scope, resolver, o, prefix);
			o << ")).Value)" << endl;
			o << prefix << "{" << endl;
			PrintStatement(current->trueBranch, scope, resolver, o, prefix + "\t", true);
			o << endl << prefix << "}";

			auto nextCurrent = dynamic_cast<AstIfStatement*>(current->falseBranch.get());
			if (!nextCurrent) break;
			o << endl << prefix << "else ";
			current = nextCurrent;
		}

		if (current->falseBranch)
		{
			o << endl << prefix << "else" << endl;
			o << prefix << "{" << endl;
			PrintStatement(current->falseBranch, scope, resolver, o, prefix + "\t", true);
			o << endl << prefix << "}";
		}
	}
};

void PrintExpression(AstExpression::Ptr expression, AstDeclaration* scope, CSharpNameResolver& resolver, ostream& o, string prefix)
{
	CSharpExpressionCodegen codegen(resolver, o, prefix, scope);
	expression->Accept(&codegen);
}

void PrintStatement(AstStatement::Ptr statement, AstDeclaration* scope, CSharpNameResolver& resolver, ostream& o, string prefix, bool block)
{
	CSharpStatementCodegen codegen(resolver, o, prefix, block, scope);
	statement->Accept(&codegen);
}

class CSharpDeclarationCodegen :public AstDeclarationVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream&				o;
	string					prefix;

	CSharpDeclarationCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
	{
	}

	void Visit(AstSymbolDeclaration* node)override
	{
		o << prefix << "public readonly TinymoeObject " << resolver.Resolve(node) << " = new TinymoeSymbol(\"" << resolver.Resolve(node) << "\");" << endl << endl;
	}

	void Visit(AstTypeDeclaration* node)override
	{
		o << prefix << "public class " << resolver.Resolve(node);
		if (node->baseType.expired())
		{
			o << " : TinymoeObject" << endl;
		}
		else
		{
			o << " : " << CSharpTypeCodegen::ToString(node->baseType.lock(), resolver) << endl;
		}
		o << prefix << "{" << endl;
		o << prefix << "\tpublic " << resolver.Resolve(node) << "()" << endl;
		o << prefix << "\t{" << endl;
		for (auto field : node->fields)
		{
			resolver.Scope(field.get(), node);
			o << prefix << "\t\tSetField(\"" << resolver.Resolve(field.get()) << "\", null);" << endl;
		}
		o << prefix << "\t}" << endl;
		o << prefix << "}" << endl << endl;
	}

	void Visit(AstFunctionDeclaration* node)override
	{
		o << prefix << "public void " << FunctionToTypedName(resolver, node) << "(";
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			resolver.Scope(it->get(), node);
			o << "TinymoeObject " << resolver.Resolve(it->get());
			if (it + 1 == node->arguments.end())
			{
				o << ")" << endl;
			}
			else
			{
				o << ", ";
			}
		}
		o << prefix << "{" << endl;
		PrintStatement(node->statement, node, resolver, o, prefix + "\t", true);
		o << endl;
		o << prefix << "}" << endl << endl;
	}
};

class CSharpExtensionDeclarationCodegen :public AstDeclarationVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream&				o;
	string					prefix;

	CSharpExtensionDeclarationCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix)
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
			o << prefix << "TinymoeObject.SetExtension(" << endl;
			o << prefix << "\ttypeof(" << typeName << ")," << endl;
			o << prefix << "\t\"" << methodName << "\"," << endl;
			o << prefix << "\t" << targetName << endl;
			o << prefix << "\t); " << endl;
		}
	}
};

void GenerateCSharpCode(AstAssembly::Ptr assembly, ostream& o)
{
	CSharpNameResolver resolver;
	o << "using System;" << endl;
	o << "using System.Collections.Generic;" << endl;
	o << "using TinymoeDotNet;" << endl;
	o << "" << endl;
	o << "namespace TinymoeProgramNamespace" << endl;
	o << "{" << endl;
	o << "\tpublic class TinymoeProgram : TinymoeOperations" << endl;
	o << "\t{" << endl;
	{
		for (auto decl : assembly->declarations)
		{
			resolver.Scope(decl.get(), nullptr);
		}
		CSharpDeclarationCodegen codegen(resolver, o, "\t\t");
		for (auto decl : assembly->declarations)
		{
			decl->Accept(&codegen);
		}
	}
	o << "\t\tpublic TinymoeProgram()" << endl;
	o << "\t\t{" << endl;
	{
		CSharpExtensionDeclarationCodegen codegen(resolver, o, "\t\t\t");
		for (auto decl : assembly->declarations)
		{
			decl->Accept(&codegen);
		}
	}
	o << "\t\t}" << endl;
	o << endl;
	{
		string mainName;
		for (auto decl : assembly->declarations)
		{
			if (decl->composedName.size() >= 6)
			{
				if (decl->composedName.substr(decl->composedName.size() - 6, 6) == "::main")
				{
					mainName = resolver.Resolve(decl.get());
					break;
				}
			}
		}
		o << "\t\tstatic void Main(string[] args)" << endl;
		o << "\t\t{" << endl;
		o << "\t\t\tvar program = new TinymoeProgram();" << endl;
		o << "\t\t\tvar continuation = new TinymoeFunction((TinymoeObject[] arguments) =>" << endl;
		o << "\t\t\t{" << endl;
		o << "\t\t\t});" << endl;
		o << "\t\t\tvar trap = new TinymoeProgram.standard_library__continuation_trap();" << endl;
		o << "\t\t\ttrap.SetField(\"continuation\", continuation);" << endl;
		o << "\t\t\tvar state = new TinymoeProgram.standard_library__continuation_state();" << endl;
		o << "\t\t\tstate.SetField(\"trap\", trap);" << endl;
		o << "\t\t\tprogram." << mainName << "(state, continuation);" << endl;
		o << "\t\t}" << endl;
	}
	o << "\t}" << endl;
	o << "}" << endl;
}